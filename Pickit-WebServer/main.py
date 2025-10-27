import os
import logging
import asyncio
from pathlib import Path
from nicegui import ui, app
import uuid
from typing import Dict

# ============================================
# CONFIGURATION - Adjust colors here
# ============================================
HEADER_COLOR = '#10b981'  # Mint green for header
BUTTON_COLOR = '#10b981'  # Mint green for buttons
PK2CMD_PATH = './pk2cmd-x86_64.AppImage'  # Path to pk2cmd executable

# ============================================
# LOGGING CONFIGURATION
# ============================================
logging.basicConfig(
    filename="weblog.log",
    format='%(asctime)s - %(levelname)s - %(message)s',
    filemode='a',
    level=logging.INFO
)
logger = logging.getLogger()

# ============================================
# GLOBAL STATE
# ============================================
programmer_busy = False
programmer_lock = asyncio.Lock()

# Track uploaded files by client connection ID
uploaded_files: Dict[str, Path] = {}

# ============================================
# FILE MANAGER
# ============================================
class FileManager:
    """Manages file uploads per client connection"""
    
    def __init__(self):
        self.upload_dir = Path('./uploads')
        self.upload_dir.mkdir(exist_ok=True)
    
    def get_client_file_path(self, client_id: str) -> Path:
        """Get the file path for a specific client"""
        return self.upload_dir / f"{client_id}.hex"
    
    def save_file(self, client_id: str, content: bytes, filename: str) -> Path:
        """Save uploaded file for a client (overwrites if exists)"""
        file_path = self.get_client_file_path(client_id)
        
        # Delete old file if exists
        if file_path.exists():
            file_path.unlink()
            logger.info(f"Overwriting existing file for client {client_id}")
        
        # Save new file
        with open(file_path, 'wb') as f:
            f.write(content)
        
        logger.info(f"File uploaded for client {client_id}: {filename} -> {file_path}")
        
        # Store in global dict for tracking
        uploaded_files[client_id] = file_path
        
        return file_path
    
    def has_file(self, client_id: str) -> bool:
        """Check if client has uploaded a file"""
        return client_id in uploaded_files and uploaded_files[client_id].exists()
    
    def get_file_path(self, client_id: str) -> Path:
        """Get the file path for a client"""
        if client_id in uploaded_files:
            return uploaded_files[client_id]
        return None

file_manager = FileManager()

# ============================================
# PROGRAMMER LOGIC
# ============================================
class Programmer:
    """Handles PIC programming with pk2cmd"""
    
    def __init__(self):
        self.last_output = ""
    
    async def program(self, hex_file_path: Path) -> dict:
        """
        Program the PIC with the given hex file
        Returns: dict with 'success' (bool) and 'output' (str)
        """
        global programmer_busy
        
        # Build command
        command = f'{PK2CMD_PATH} -B. -P -F"{hex_file_path}" -M -Y'
        logger.info(f"Starting programming with command: {command}")
        
        try:
            # Execute command asynchronously
            process = await asyncio.create_subprocess_shell(
                command,
                stdout=asyncio.subprocess.PIPE,
                stderr=asyncio.subprocess.PIPE
            )
            
            stdout, stderr = await process.communicate()
            output = stdout.decode() + stderr.decode()
            self.last_output = output
            
            logger.info(f"Programming completed with return code: {process.returncode}")
            logger.info(f"Output: {output}")
            
            # Determine success based on output and return code
            success = process.returncode == 0 and "Failed" not in output
            
            if success:
                logger.info("Programming succeeded")
            else:
                logger.error("Programming failed")
            
            return {
                'success': success,
                'output': output
            }
            
        except Exception as e:
            error_msg = f"Error executing pk2cmd: {str(e)}"
            logger.error(error_msg)
            self.last_output = error_msg
            return {
                'success': False,
                'output': error_msg
            }
    
    def get_last_output(self) -> str:
        """Get the output from the last programming operation"""
        return self.last_output if self.last_output else "No programming operations yet"

programmer = Programmer()

# ============================================
# UI COMPONENTS
# ============================================
class UIComponents:
    """Reusable UI components and dialogs"""
    
    @staticmethod
    def create_confirmation_dialog(on_program, on_cancel):
        """Create the confirmation dialog before programming"""
        with ui.dialog() as dialog, ui.card().classes('p-6 items-center').style('min-width: 400px; max-width: 90vw;'):
            ui.label('Program PIC ⚠️').classes('text-2xl font-bold mb-4')
            
            # Image
            ui.image('Programer.png').classes('w-48 h-48 object-contain mb-4')
            
            # Instructions
            ui.label(
                'Please make sure the PIC is in the programer like the image indicates '
                'and the lever is fully closed before programing'
            ).classes('text-center mb-6').style('max-width: 350px;')
            
            # Buttons
            with ui.row().classes('w-full justify-end gap-2'):
                ui.button('Cancel', on_click=lambda: dialog.close()).props('flat').classes('text-gray-600')
                ui.button('Program', on_click=lambda: [dialog.close(), on_program()]).style(
                    f'background-color: {BUTTON_COLOR};'
                )
        
        return dialog
    
    @staticmethod
    def create_programming_dialog():
        """Create the programming in progress dialog"""
        with ui.dialog() as dialog, ui.card().classes('p-8 items-center').style('min-width: 350px; max-width: 90vw;'):
            ui.label('Programming').classes('text-2xl font-bold mb-6')
            ui.spinner(size='xl', color=BUTTON_COLOR).classes('mb-4')
            ui.label('Do not disconnect the PIC').classes('text-center text-gray-600')
        
        dialog.props('persistent')  # Cannot be closed by clicking outside
        return dialog
    
    @staticmethod
    def create_success_dialog(on_logs, on_close):
        """Create the success dialog"""
        with ui.dialog() as dialog, ui.card().classes('p-6 items-center').style('min-width: 350px; max-width: 90vw;'):
            ui.label('Successfully programmed').classes('text-2xl font-bold mb-4')
            
            # Success icon
            with ui.card().classes('w-24 h-24 bg-green-100 items-center justify-center mb-4 rounded-full'):
                ui.label('✓').classes('text-5xl text-green-600')
            
            ui.label('The PIC has been programmed successfully').classes('text-center text-gray-600 mb-6')
            
            # Buttons
            with ui.row().classes('w-full justify-end gap-2'):
                ui.button('Logs', on_click=lambda: [dialog.close(), on_logs()]).props('flat').classes('text-gray-600')
                ui.button('Close', on_click=lambda: dialog.close()).style(
                    f'background-color: {BUTTON_COLOR};'
                )
        
        return dialog
    
    @staticmethod
    def create_failed_dialog(on_logs, on_close):
        """Create the failed dialog"""
        with ui.dialog() as dialog, ui.card().classes('p-6 items-center').style('min-width: 350px; max-width: 90vw;'):
            ui.label('Programming failed').classes('text-2xl font-bold mb-4')
            
            # Error icon
            with ui.card().classes('w-24 h-24 bg-red-100 items-center justify-center mb-4 rounded-full'):
                ui.label('✕').classes('text-5xl text-red-600')
            
            ui.label('Check LOGS for more detail').classes('text-center text-gray-600 mb-6 font-mono text-sm')
            
            # Buttons
            with ui.row().classes('w-full justify-end gap-2'):
                ui.button('Logs', on_click=lambda: [dialog.close(), on_logs()]).props('flat').classes('text-gray-600')
                ui.button('Close', on_click=lambda: dialog.close()).style(
                    f'background-color: {BUTTON_COLOR};'
                )
        
        return dialog
    
    @staticmethod
    def create_logs_dialog():
        """Create the logs dialog"""
        with ui.dialog() as dialog, ui.card().classes('p-6').style('min-width: 600px; max-width: 90vw; max-height: 80vh;'):
            ui.label('Programmer logs').classes('text-2xl font-bold mb-4')
            
            # Logs content (scrollable)
            log_content = ui.label('No logs available yet').classes('font-mono text-sm whitespace-pre-wrap bg-gray-100 p-4 rounded overflow-auto').style(
                'max-height: 400px; width: 100%;'
            )
            
            # Store reference to update function
            dialog.log_content = log_content
            
            # Close button
            with ui.row().classes('w-full justify-end mt-4'):
                ui.button('Close', on_click=lambda: dialog.close()).style(
                    f'background-color: {BUTTON_COLOR};'
                )
        
        return dialog

# ============================================
# MAIN APPLICATION
# ============================================
def create_main_page():
    """Create the main application page"""
    
    # Get client ID from NiceGUI (unique per connection)
    client_id = app.storage.client.get('id')
    if not client_id:
        client_id = str(uuid.uuid4())
        app.storage.client['id'] = client_id
    
    logger.info(f"Client connected with ID: {client_id}")
    
    # File upload handler
    def handle_upload(e):
        try:
            logger.info(f"Upload event triggered for client {client_id}")
            
            # In NiceGUI, the file data is directly in the event object
            if not hasattr(e, 'name') or not e.name:
                logger.error(f"No filename in upload event for client {client_id}")
                ui.notify('Upload failed: No file received', type='negative')
                return
            
            filename = e.name
            logger.info(f"Uploading file: {filename}")
            
            # Check if it's a .hex file
            if not filename.lower().endswith('.hex'):
                ui.notify('Only .hex files are allowed', type='negative')
                logger.warning(f"Client {client_id} attempted to upload non-hex file: {filename}")
                return
            
            # Read file content from the event
            content = e.content.read()
            logger.info(f"File content length: {len(content)} bytes")
            
            # Save file
            saved_path = file_manager.save_file(client_id, content, filename)
            ui.notify(f'File uploaded: {filename}', type='positive')
            upload_label.text = f'Uploaded: {filename}'
            logger.info(f"Client {client_id} successfully uploaded: {filename} to {saved_path}")
            
        except Exception as ex:
            logger.error(f"Error in handle_upload for client {client_id}: {str(ex)}")
            logger.exception(ex)
            ui.notify(f'Upload error: {str(ex)}', type='negative')
    
    # Programming flow
    async def start_programming():
        global programmer_busy
        
        # Check if file is uploaded
        if not file_manager.has_file(client_id):
            ui.notify('Nothing to program', type='warning')
            logger.warning(f"Client {client_id} attempted to program without uploading a file")
            return
        
        # Check if programmer is busy
        if programmer_busy:
            ui.notify('Programmer busy', type='warning')
            logger.warning(f"Client {client_id} attempted to program while programmer is busy")
            return
        
        # Show confirmation dialog
        confirmation_dialog.open()
    
    async def execute_programming():
        global programmer_busy
        
        async with programmer_lock:
            programmer_busy = True
            logger.info(f"Client {client_id} started programming")
            
            # Show programming dialog
            programming_dialog.open()
            
            try:
                # Get file path and program
                file_path = file_manager.get_file_path(client_id)
                result = await programmer.program(file_path)
                
                # Close programming dialog
                programming_dialog.close()
                
                # Show result dialog
                if result['success']:
                    success_dialog.open()
                else:
                    failed_dialog.open()
                    
            except Exception as ex:
                logger.error(f"Error during programming for client {client_id}: {str(ex)}")
                logger.exception(ex)
                programming_dialog.close()
                failed_dialog.open()
            finally:
                programmer_busy = False
                logger.info(f"Client {client_id} finished programming")
    
    def show_logs():
        # Update logs content before opening
        logs_dialog.log_content.text = programmer.get_last_output()
        logs_dialog.open()
        logger.info(f"Client {client_id} opened logs")
    
    # ============================================
    # PAGE LAYOUT
    # ============================================
    
    # Create all dialogs upfront (before any async operations)
    confirmation_dialog = UIComponents.create_confirmation_dialog(
        on_program=lambda: asyncio.create_task(execute_programming()),
        on_cancel=lambda: logger.info(f"Client {client_id} cancelled programming")
    )
    
    programming_dialog = UIComponents.create_programming_dialog()
    
    success_dialog = UIComponents.create_success_dialog(
        on_logs=show_logs,
        on_close=lambda: logger.info(f"Client {client_id} closed success dialog")
    )
    
    failed_dialog = UIComponents.create_failed_dialog(
        on_logs=show_logs,
        on_close=lambda: logger.info(f"Client {client_id} closed failed dialog")
    )
    
    logs_dialog = UIComponents.create_logs_dialog()
    
    # Header
    with ui.header().style(f'background-color: {HEADER_COLOR};').classes('items-center justify-center'):
        ui.label('Pickit Programing server').classes('text-2xl font-bold')
    
    # Main content
    with ui.column().classes('w-full items-center justify-center p-8').style('min-height: calc(100vh - 64px);'):
        
        # Upload area
        with ui.card().classes('p-6 items-center').style('min-width: 300px; max-width: 500px; width: 90vw;'):
            ui.label('Upload').classes('text-lg mb-2')
            ui.label('Upload a production hex file for PIC to program').classes('text-sm text-gray-600 mb-4')
            
            # Upload button
            ui.upload(
                on_upload=handle_upload,
                auto_upload=True,
                multiple=False
            ).props('accept=.hex').classes('mb-2').style('width: 100%;')
            
            # Upload status label
            upload_label = ui.label('No file uploaded').classes('text-sm text-gray-500')
        
        # Program button
        ui.button('Program', on_click=start_programming).classes('mt-8').style(
            f'background-color: {BUTTON_COLOR}; padding: 12px 48px; font-size: 16px;'
        ).props('icon=o_memory')

# ============================================
# APPLICATION ENTRY POINT
# ============================================
@ui.page('/')
def index():
    create_main_page()

# Run the application
ui.run(
    title='PicKit Web Programmer',
    port=8080,
    reload=True,
    show=False
)
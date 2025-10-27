#!/usr/bin/env python3
"""
PK2CMD File Programmer
A NiceGUI application for uploading and programming hex files using pk2cmd
"""

import os
import subprocess
from pathlib import Path
from nicegui import ui, app

class PK2Programmer:
    def __init__(self):
        self.upload_dir = Path("uploads")
        self.upload_dir.mkdir(exist_ok=True)
        self.current_file = None
        self.log_area = None
        self.upload_card = None
        self.result_card = None
        
    def setup_ui(self):
        """Set up the user interface"""
        ui.label("PK2CMD File Programmer").classes("text-3xl font-bold mb-4")
        
        # Upload card (initially visible)
        with ui.card().classes("w-full max-w-2xl") as self.upload_card:
            ui.label("Upload HEX File").classes("text-xl font-semibold mb-2")
            ui.label("Select a .hex file to program with pk2cmd").classes("text-gray-600 mb-4")
            
            upload = ui.upload(
                on_upload=self.handle_upload,
                auto_upload=True,
                label="Choose HEX file"
            ).props('accept=".hex"').classes("w-full")
            
            self.upload_button = ui.button(
                "Program Device",
                on_click=self.program_device
            ).props("color=primary size=lg").classes("w-full mt-4")
            self.upload_button.set_enabled(False)
            
            self.status_label = ui.label("").classes("mt-2 text-sm")
        
        # Result card (initially hidden)
        with ui.card().classes("w-full max-w-2xl") as self.result_card:
            self.result_card.set_visibility(False)
            
            ui.label("Programming Log").classes("text-xl font-semibold mb-2")
            
            self.log_area = ui.textarea().classes("w-full font-mono text-sm").props(
                "readonly outlined rows=20"
            )
            
            ui.button(
                "Upload Another File",
                on_click=self.reset_ui
            ).props("color=primary size=lg").classes("w-full mt-4")
    
    def handle_upload(self, e):
        """Handle file upload"""
        try:
            # Save the uploaded file
            filename = e.name
            self.current_file = self.upload_dir / filename
            
            with open(self.current_file, 'wb') as f:
                f.write(e.content.read())
            
            self.status_label.text = f"✓ File uploaded: {filename}"
            self.status_label.classes("text-green-600")
            self.upload_button.set_enabled(True)
            
        except Exception as ex:
            self.status_label.text = f"✗ Upload error: {str(ex)}"
            self.status_label.classes("text-red-600")
            self.upload_button.set_enabled(False)
    
    async def program_device(self):
        """Execute pk2cmd with the uploaded file"""
        if not self.current_file or not self.current_file.exists():
            ui.notify("No file uploaded!", color="negative")
            return
        
        self.status_label.text = "Programming device..."
        self.status_label.classes("text-blue-600")
        self.upload_button.set_enabled(False)
        
        try:
            # Build the command
            cmd = [
                "./pk2cmd",
                "-B.",
                "-P",
                "-E",
                f"-F{self.current_file}",
                "-M"
            ]
            
            # Execute the command
            result = subprocess.run(
                cmd,
                capture_output=True,
                text=True,
                timeout=60
            )
            
            # Combine stdout and stderr
            output = result.stdout
            if result.stderr:
                output += f"\n--- STDERR ---\n{result.stderr}"
            
            if result.returncode == 0:
                output += f"\n\n✓ Command completed successfully (exit code: {result.returncode})"
            else:
                output += f"\n\n✗ Command failed with exit code: {result.returncode}"
            
            # Show results
            self.log_area.value = output
            self.upload_card.set_visibility(False)
            self.result_card.set_visibility(True)
            
            # Clean up the file
            self.cleanup_file()
            
            if result.returncode == 0:
                ui.notify("Programming completed successfully!", color="positive")
            else:
                ui.notify("Programming failed. Check the log.", color="warning")
            
        except subprocess.TimeoutExpired:
            self.log_area.value = "✗ Command timed out after 60 seconds"
            self.upload_card.set_visibility(False)
            self.result_card.set_visibility(True)
            self.cleanup_file()
            ui.notify("Command timed out!", color="negative")
            
        except FileNotFoundError:
            error_msg = "✗ Error: pk2cmd not found in current directory\n\n"
            error_msg += "Make sure pk2cmd is in the same directory as this script and is executable."
            self.log_area.value = error_msg
            self.upload_card.set_visibility(False)
            self.result_card.set_visibility(True)
            self.cleanup_file()
            ui.notify("pk2cmd not found!", color="negative")
            
        except Exception as ex:
            self.log_area.value = f"✗ Unexpected error: {str(ex)}"
            self.upload_card.set_visibility(False)
            self.result_card.set_visibility(True)
            self.cleanup_file()
            ui.notify(f"Error: {str(ex)}", color="negative")
    
    def cleanup_file(self):
        """Delete the uploaded file"""
        try:
            if self.current_file and self.current_file.exists():
                self.current_file.unlink()
                print(f"Deleted file: {self.current_file}")
        except Exception as ex:
            print(f"Error deleting file: {ex}")
    
    def reset_ui(self):
        """Reset UI to upload another file"""
        self.current_file = None
        self.upload_button.set_enabled(False)
        self.status_label.text = ""
        self.log_area.value = ""
        self.upload_card.set_visibility(True)
        self.result_card.set_visibility(False)

# Initialize and run the application
programmer = PK2Programmer()
programmer.setup_ui()

# Clean up uploads directory on shutdown
@app.on_shutdown
def cleanup():
    """Clean up any remaining files on shutdown"""
    upload_dir = Path("uploads")
    if upload_dir.exists():
        for file in upload_dir.glob("*"):
            try:
                file.unlink()
            except:
                pass

ui.run(title="PK2CMD Programmer", port=8181, reload=False)
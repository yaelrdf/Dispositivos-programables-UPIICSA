import os
import sys
import subprocess
from pathlib import Path

def list_hex_files(folder):
    """List all .hex files in the given folder"""
    hex_files = sorted([f for f in os.listdir(folder) if f.lower().endswith('.hex')])
    return hex_files

def display_files(hex_files):
    """Display numbered list of hex files"""
    print("\n" + "="*50)
    print("Available HEX files:")
    print("="*50)
    for i, filename in enumerate(hex_files, 1):
        print(f"{i}. {filename}")
    print("="*50)

def program_hex(hex_file):
    """Program the selected hex file using pk2cmd"""
    script_dir = os.path.dirname(os.path.abspath(__file__))
    pk2cmd_path = os.path.join(script_dir, 'pk2cmd.exe')
    
    cmd = [pk2cmd_path, '-B.', '-P', '-E', f'-F{hex_file}', '-M', '-R']
    
    print(f"\nProgramming {hex_file}...")
    print(f"Command: {' '.join(cmd)}\n")
    
    try:
        result = subprocess.run(cmd, check=True, text=True, capture_output=True)
        print(result.stdout)
        if result.stderr:
            print(result.stderr)
        print("\nProgramming completed successfully!")
    except subprocess.CalledProcessError as e:
        print(f"\nError during programming:")
        print(e.stdout)
        print(e.stderr)
    except FileNotFoundError:
        print(f"\nError: pk2cmd not found at {pk2cmd_path}")
        print("Make sure pk2cmd is in the same directory as this script.")

def main():
    if len(sys.argv) != 2:
        print("Usage: python script.py <folder_with_hex_files>")
        sys.exit(1)
    
    folder = sys.argv[1]
    
    if not os.path.isdir(folder):
        print(f"Error: '{folder}' is not a valid directory")
        sys.exit(1)
    
    while True:
        hex_files = list_hex_files(folder)
        
        if not hex_files:
            print(f"No .hex files found in '{folder}'")
            sys.exit(0)
        
        display_files(hex_files)
        
        try:
            choice = input("\nEnter file number to program (or 'q' to quit): ").strip()
            
            if choice.lower() == 'q':
                print("Exiting...")
                break
            
            file_num = int(choice)
            
            if 1 <= file_num <= len(hex_files):
                selected_file = hex_files[file_num - 1]
                full_path = os.path.join(folder, selected_file)
                
                confirm = input(f"\nProgram '{selected_file}'? (y/n): ").strip().lower()
                
                if confirm == 'y':
                    program_hex(full_path)
                    input("\nPress Enter to continue...")
                else:
                    print("Programming cancelled.")
            else:
                print(f"Invalid number. Please enter a number between 1 and {len(hex_files)}")
        
        except ValueError:
            print("Invalid input. Please enter a number or 'q' to quit.")
        except KeyboardInterrupt:
            print("\n\nExiting...")
            break

if __name__ == "__main__":
    main()
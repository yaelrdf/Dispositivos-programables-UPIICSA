# The following variables contains the files used by the different stages of the build process.
set(Fecha_default_default_PIC_AS_FILE_TYPE_assemble "${CMAKE_CURRENT_SOURCE_DIR}/../../../Fecha.X/main.s")
set_source_files_properties(${Fecha_default_default_PIC_AS_FILE_TYPE_assemble} PROPERTIES LANGUAGE ASM)

# For assembly files, add "." to the include path for each file so that .include with a relative path works
foreach(source_file ${Fecha_default_default_PIC_AS_FILE_TYPE_assemble})
        set_source_files_properties(${source_file} PROPERTIES INCLUDE_DIRECTORIES "$<PATH:NORMAL_PATH,$<PATH:REMOVE_FILENAME,${source_file}>>")
endforeach()

set(Fecha_default_default_PIC_AS_FILE_TYPE_link)
set(Fecha_default_image_name "default-production.elf")
set(Fecha_default_image_base_name "default-production")

# The output directory of the final image.
set(Fecha_default_output_dir "${CMAKE_CURRENT_SOURCE_DIR}/../../../out/Fecha/production")

# The full path to the final image.
set(Fecha_default_full_path_to_image ${Fecha_default_output_dir}/${Fecha_default_image_name})

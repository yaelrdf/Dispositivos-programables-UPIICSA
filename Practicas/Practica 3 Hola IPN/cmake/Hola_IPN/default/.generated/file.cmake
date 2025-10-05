# The following variables contains the files used by the different stages of the build process.
set(Hola_IPN_default_default_PIC_AS_FILE_TYPE_assemble
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Hola_IPN.X/DisplayCode.s"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Hola_IPN.X/main.s")
set_source_files_properties(${Hola_IPN_default_default_PIC_AS_FILE_TYPE_assemble} PROPERTIES LANGUAGE ASM)

# For assembly files, add "." to the include path for each file so that .include with a relative path works
foreach(source_file ${Hola_IPN_default_default_PIC_AS_FILE_TYPE_assemble})
        set_source_files_properties(${source_file} PROPERTIES INCLUDE_DIRECTORIES "$<PATH:NORMAL_PATH,$<PATH:REMOVE_FILENAME,${source_file}>>")
endforeach()

set(Hola_IPN_default_default_PIC_AS_FILE_TYPE_link)
set(Hola_IPN_default_image_name "default.elf")
set(Hola_IPN_default_image_base_name "default")

# The output directory of the final image.
set(Hola_IPN_default_output_dir "${CMAKE_CURRENT_SOURCE_DIR}/../../../out/Hola_IPN")

# The full path to the final image.
set(Hola_IPN_default_full_path_to_image ${Hola_IPN_default_output_dir}/${Hola_IPN_default_image_name})

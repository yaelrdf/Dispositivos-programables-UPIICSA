# cmake files support debug production
include("${CMAKE_CURRENT_LIST_DIR}/rule.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/file.cmake")

set(Fecha_default_library_list )

# Handle files with suffix (s|S|asm|ASM|msa|MSA), for group default-PIC-AS
if(Fecha_default_default_PIC_AS_FILE_TYPE_assemble)
add_library(Fecha_default_default_PIC_AS_assemble OBJECT ${Fecha_default_default_PIC_AS_FILE_TYPE_assemble})
    Fecha_default_default_PIC_AS_assemble_rule(Fecha_default_default_PIC_AS_assemble)
    list(APPEND Fecha_default_library_list "$<TARGET_OBJECTS:Fecha_default_default_PIC_AS_assemble>")
endif()

add_executable(Fecha_default_image_zGLw9Epo ${Fecha_default_library_list})

set_target_properties(Fecha_default_image_zGLw9Epo PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${Fecha_default_output_dir})
set_target_properties(Fecha_default_image_zGLw9Epo PROPERTIES OUTPUT_NAME "default")
set_target_properties(Fecha_default_image_zGLw9Epo PROPERTIES SUFFIX ".elf")

target_link_libraries(Fecha_default_image_zGLw9Epo PRIVATE ${Fecha_default_default_PIC_AS_FILE_TYPE_link})


# Add the link options from the rule file.
Fecha_default_link_rule(Fecha_default_image_zGLw9Epo)




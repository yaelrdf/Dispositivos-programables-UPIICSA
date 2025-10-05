# cmake files support debug production
include("${CMAKE_CURRENT_LIST_DIR}/rule.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/file.cmake")

set(Contrasena_default_library_list )

# Handle files with suffix (s|S|asm|ASM|msa|MSA), for group default-PIC-AS
if(Contrasena_default_default_PIC_AS_FILE_TYPE_assemble)
add_library(Contrasena_default_default_PIC_AS_assemble OBJECT ${Contrasena_default_default_PIC_AS_FILE_TYPE_assemble})
    Contrasena_default_default_PIC_AS_assemble_rule(Contrasena_default_default_PIC_AS_assemble)
    list(APPEND Contrasena_default_library_list "$<TARGET_OBJECTS:Contrasena_default_default_PIC_AS_assemble>")
endif()

add_executable(Contrasena_default_image_w7wlzkrg ${Contrasena_default_library_list})

set_target_properties(Contrasena_default_image_w7wlzkrg PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${Contrasena_default_output_dir})
set_target_properties(Contrasena_default_image_w7wlzkrg PROPERTIES OUTPUT_NAME "default")
set_target_properties(Contrasena_default_image_w7wlzkrg PROPERTIES SUFFIX ".elf")

target_link_libraries(Contrasena_default_image_w7wlzkrg PRIVATE ${Contrasena_default_default_PIC_AS_FILE_TYPE_link})


# Add the link options from the rule file.
Contrasena_default_link_rule(Contrasena_default_image_w7wlzkrg)




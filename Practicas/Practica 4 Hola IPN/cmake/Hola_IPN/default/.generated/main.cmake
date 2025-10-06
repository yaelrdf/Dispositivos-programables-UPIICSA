# cmake files support debug production
include("${CMAKE_CURRENT_LIST_DIR}/rule.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/file.cmake")

set(Hola_IPN_default_library_list )

# Handle files with suffix (s|S|asm|ASM|msa|MSA), for group default-PIC-AS
if(Hola_IPN_default_default_PIC_AS_FILE_TYPE_assemble)
add_library(Hola_IPN_default_default_PIC_AS_assemble OBJECT ${Hola_IPN_default_default_PIC_AS_FILE_TYPE_assemble})
    Hola_IPN_default_default_PIC_AS_assemble_rule(Hola_IPN_default_default_PIC_AS_assemble)
    list(APPEND Hola_IPN_default_library_list "$<TARGET_OBJECTS:Hola_IPN_default_default_PIC_AS_assemble>")
endif()

add_executable(Hola_IPN_default_image_8mG9GbJB ${Hola_IPN_default_library_list})

set_target_properties(Hola_IPN_default_image_8mG9GbJB PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${Hola_IPN_default_output_dir})
set_target_properties(Hola_IPN_default_image_8mG9GbJB PROPERTIES OUTPUT_NAME "default")
set_target_properties(Hola_IPN_default_image_8mG9GbJB PROPERTIES SUFFIX ".elf")

target_link_libraries(Hola_IPN_default_image_8mG9GbJB PRIVATE ${Hola_IPN_default_default_PIC_AS_FILE_TYPE_link})


# Add the link options from the rule file.
Hola_IPN_default_link_rule(Hola_IPN_default_image_8mG9GbJB)




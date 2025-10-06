# cmake files support debug production
include("${CMAKE_CURRENT_LIST_DIR}/rule.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/file.cmake")

set(Hola_IPN_default_library_list )

# Handle files with suffix (s|as|asm|AS|ASM|As|aS|Asm), for group default-XC8
if(Hola_IPN_default_default_XC8_FILE_TYPE_assemble)
add_library(Hola_IPN_default_default_XC8_assemble OBJECT ${Hola_IPN_default_default_XC8_FILE_TYPE_assemble})
    Hola_IPN_default_default_XC8_assemble_rule(Hola_IPN_default_default_XC8_assemble)
    list(APPEND Hola_IPN_default_library_list "$<TARGET_OBJECTS:Hola_IPN_default_default_XC8_assemble>")
endif()

# Handle files with suffix S, for group default-XC8
if(Hola_IPN_default_default_XC8_FILE_TYPE_assemblePreprocess)
add_library(Hola_IPN_default_default_XC8_assemblePreprocess OBJECT ${Hola_IPN_default_default_XC8_FILE_TYPE_assemblePreprocess})
    Hola_IPN_default_default_XC8_assemblePreprocess_rule(Hola_IPN_default_default_XC8_assemblePreprocess)
    list(APPEND Hola_IPN_default_library_list "$<TARGET_OBJECTS:Hola_IPN_default_default_XC8_assemblePreprocess>")
endif()

# Handle files with suffix [cC], for group default-XC8
if(Hola_IPN_default_default_XC8_FILE_TYPE_compile)
add_library(Hola_IPN_default_default_XC8_compile OBJECT ${Hola_IPN_default_default_XC8_FILE_TYPE_compile})
    Hola_IPN_default_default_XC8_compile_rule(Hola_IPN_default_default_XC8_compile)
    list(APPEND Hola_IPN_default_library_list "$<TARGET_OBJECTS:Hola_IPN_default_default_XC8_compile>")
endif()

add_executable(Hola_IPN_default_image_cgQNNq1X ${Hola_IPN_default_library_list})

set_target_properties(Hola_IPN_default_image_cgQNNq1X PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${Hola_IPN_default_output_dir})
set_target_properties(Hola_IPN_default_image_cgQNNq1X PROPERTIES OUTPUT_NAME "default")
set_target_properties(Hola_IPN_default_image_cgQNNq1X PROPERTIES SUFFIX ".elf")

target_link_libraries(Hola_IPN_default_image_cgQNNq1X PRIVATE ${Hola_IPN_default_default_XC8_FILE_TYPE_link})


# Add the link options from the rule file.
Hola_IPN_default_link_rule(Hola_IPN_default_image_cgQNNq1X)




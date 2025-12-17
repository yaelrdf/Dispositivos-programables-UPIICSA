/*****************************************************************************
 * BASE DE DATOS DE USUARIOS - CONTROL DE ACCESO RFID
 * Archivo: users.h
 * 
 * INSTRUCCIONES:
 * ==============
 * 1. Agrega nuevos usuarios al array userDatabase[]
 * 2. Actualiza USER_COUNT con el número total de usuarios
 * 3. El UID debe ser EXACTAMENTE 8 caracteres hexadecimales (0-9, A-F)
 * 4. El nombre puede tener hasta 20 caracteres
 * 
 * CÓMO OBTENER EL UID DE TU TARJETA:
 * ===================================
 * 1. Compila y carga el programa principal
 * 2. Abre el monitor serial (9600 baud)
 * 3. Coloca una tarjeta nueva en el lector
 * 4. El sistema mostrará "ACCESO DENEGADO" y el UID desconocido
 * 5. Copia ese UID y agrégalo aquí con el nombre del usuario
 * 
 * EJEMPLO DE SALIDA EN MONITOR SERIAL:
 * >>> Tarjeta detectada
 * UID: A1B2C3D4
 * Estado: ACCESO DENEGADO - UID no registrado
 * 
 * FORMATO DEL UID:
 * ================
 * - Debe ser MAYÚSCULAS
 * - 8 caracteres hexadecimales
 * - Sin espacios ni guiones
 * - Ejemplos válidos: "A1B2C3D4", "12345678", "DEADBEEF"
 * - Ejemplos inválidos: "a1b2c3d4", "A1-B2-C3-D4", "123"
 *****************************************************************************/

#ifndef USERS_H
#define USERS_H

#include <stdint.h>

// ===================== ESTRUCTURA DE USUARIO =====================
typedef struct {
    const char uid[9];      // 8 caracteres + null terminator
    const char name[21];    // 20 caracteres + null terminator
} UserEntry;

// ===================== BASE DE DATOS =====================

// IMPORTANTE: Actualiza este número cuando agregues/elimines usuarios
#define USER_COUNT 10

// Array de usuarios autorizados
const UserEntry userDatabase[USER_COUNT] = {
    // UID         Nombre del Usuario
    // --------    ------------------
    {"A1B2C3D4",   "Juan Perez"},
    {"E5F6A7B8",   "Maria Lopez"},
    {"12345678",   "Admin"},
    {"9ABCDEF0",   "Carlos Gomez"},
    {"FEEDFACE",   "Ana Martinez"},
    {"DEADBEEF",   "Luis Rodriguez"},
    {"C0FFEE00",   "Sofia Torres"},
    {"BAADF00D",   "Miguel Angel"},
    {"FACADE01",   "Laura Sanchez"},
    {"DECADE02",   "Pedro Ramirez"}
};

// ===================== NOTAS ADICIONALES =====================
/*
 * LÍMITES:
 * - Máximo 50 usuarios (puedes cambiar MAX_USERS en main.c)
 * - UID: 8 caracteres exactos
 * - Nombre: hasta 20 caracteres
 * 
 * SEGURIDAD:
 * - Este archivo se compila en el firmware
 * - Para actualizar usuarios, debes recompilar y cargar el programa
 * - Para un sistema más dinámico, considera usar EEPROM externa
 * 
 * BACKUP:
 * - Mantén una copia de este archivo
 * - Documenta qué UID pertenece a qué tarjeta física
 * 
 * EJEMPLO DE DOCUMENTACIÓN PERSONAL:
 * UID: A1B2C3D4 -> Tarjeta blanca #1 (llavero azul)
 * UID: E5F6A7B8 -> Tarjeta roja #2 (tarjeta de empleado)
 */

#endif // USERS_H
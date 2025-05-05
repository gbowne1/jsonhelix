#include "settings.h"
#include <string.h>

// Schema for package.json
static SchemaField package_json_schema[] = {
    {"name", JSON_STRING, 1},
    {"version", JSON_STRING, 1},
    {"description", JSON_STRING, 0},
    {"main", JSON_STRING, 0},
    {"scripts", JSON_OBJECT, 0},
    {"dependencies", JSON_OBJECT, 0},
    {"devDependencies", JSON_OBJECT, 0}
};

// Schema for VSCode settings.json
static SchemaField vscode_settings_schema[] = {
    {"editor.fontSize", JSON_NUMBER, 0},
    {"editor.tabSize", JSON_NUMBER, 0},
    {"files.autoSave", JSON_STRING, 0},
    {"terminal.integrated.shell.linux", JSON_STRING, 0}
};

// Schema for jsonhelix config
static SchemaField jsonhelix_schema[] = {
    {"theme", JSON_STRING, 0},
    {"autosave", JSON_BOOL, 0},
    {"lintOnSave", JSON_BOOL, 0}
};

ValidationError* validate_package_json(const JsonValue *root) {
    return validate_json(root, package_json_schema, sizeof(package_json_schema) / sizeof(SchemaField));
}

ValidationError* validate_vscode_settings(const JsonValue *root) {
    return validate_json(root, vscode_settings_schema, sizeof(vscode_settings_schema) / sizeof(SchemaField));
}

ValidationError* validate_jsonhelix_config(const JsonValue *root) {
    return validate_json(root, jsonhelix_schema, sizeof(jsonhelix_schema) / sizeof(SchemaField));
}

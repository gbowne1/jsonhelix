#ifndef SETTINGS_H
#define SETTINGS_H

#include "json_parser.h"
#include "json_validator.h"

// Validate a given JSON string as a package.json
ValidationError* validate_package_json(const JsonValue *root);

// Validate a .vscode/settings.json file
ValidationError* validate_vscode_settings(const JsonValue *root);

// Future: custom project config
ValidationError* validate_jsonhelix_config(const JsonValue *root);

#endif

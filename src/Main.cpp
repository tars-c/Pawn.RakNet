/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2020 urShadow
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "SDK/amx/amx.h"
#include "SDK/plugincommon.h"

#include "RakNet/BitStream.h"
#include "RakNet/StringCompressor.h"
#include "RakNet/PluginInterface.h"
#include "urmem/urmem.hpp"
#include "crtp_singleton/crtp_singleton.hpp"
#include "cpptoml/include/cpptoml.h"

#include <unordered_set>
#include <list>
#include <array>
#include <string>
#include <regex>
#include <queue>

#include "Pawn.RakNet.inc"

extern void *pAMXFunctions;
using logprintf_t = void(*)(const char *format, ...);

#include "Logger.h"
#include "Settings.h"
#include "Addresses.h"
#include "Functions.h"
#include "Scripts.h"
#include "Hooks.h"
#include "Natives.h"

namespace Plugin {
    bool Load(void **ppData) {
        pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];

        Logger::instance()->Init(reinterpret_cast<logprintf_t>(ppData[PLUGIN_DATA_LOGPRINTF]), "[" + std::string(Settings::kPluginName) + "] ");

        Logger::instance()->Write(
            "\n\n" \
            "    | %s %s | 2016 - %s" \
            "\n" \
            "    |--------------------------------" \
            "\n" \
            "    | Author and maintainer: urShadow" \
            "\n\n\n" \
            "    | Compiled: %s at %s" \
            "\n" \
            "    |--------------------------------------------------------------" \
            "\n" \
            "    | Forum thread: %s" \
            "\n" \
            "    |--------------------------------------------------------------" \
            "\n" \
            "    | Repository: %s" \
            "\n" \
            "    |--------------------------------------------------------------" \
            "\n" \
            "    | Wiki: %s/wiki" \
            "\n",
            Settings::kPluginName,
            Settings::kPluginVersion,
            &__DATE__[7],
            __DATE__,
            __TIME__,
            Settings::kPluginThreadUrl,
            Settings::kPluginRepositoryUrl,
            Settings::kPluginRepositoryUrl
        );

        try {
            Settings::Read();

            Hooks::Init(*ppData);

            StringCompressor::AddReference();

            return true;
        } catch (const std::exception &e) {
            Logger::instance()->Write("%s: %s", __FUNCTION__, e.what());
        }

        return false;
    }

    void Unload() {
        try {
            StringCompressor::RemoveReference();

            Settings::Save();

            Logger::instance()->Write("plugin unloaded");
        } catch (const std::exception &e) {
            Logger::instance()->Write("%s: %s", __FUNCTION__, e.what());
        }
    }

    void AmxLoad(AMX *amx) {
        cell include_version{};
        if (Functions::GetAmxPublicVar(amx, Settings::kPublicVarNameVersion, include_version)) {
            if (include_version != PAWNRAKNET_INCLUDE_VERSION) {
                Logger::instance()->Write("%s: mismatch between the plugin (%d) and include (%d) versions", __FUNCTION__, PAWNRAKNET_INCLUDE_VERSION, include_version);

                return;
            }

            Natives::Register(amx);
        }
    }
};

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports() {
    return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES;
}

PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData) {
    return Plugin::Load(ppData);
}

PLUGIN_EXPORT void PLUGIN_CALL Unload() {
    Plugin::Unload();
}

PLUGIN_EXPORT void PLUGIN_CALL AmxLoad(AMX *amx) {
    Plugin::AmxLoad(amx);
}

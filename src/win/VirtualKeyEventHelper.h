/**
 * VirtualKeyEventHelper.h
 *
 * Utility for generating Ultralight keyboard events from Windows virtual key codes,
 * with proper keyboard layout translation and state tracking.
 *
 * Copyright (C) 2025 Ultralight, Inc.
 */

#pragma once

#ifdef _WIN32

#include <Ultralight/Ultralight.h>
#include <Windows.h>
#include <unordered_set>
#include <vector>

namespace ultralight {

/**
 * Generates Ultralight keyboard events from Windows virtual key codes (polls underlying OS
 * keyboard state for proper layout translation and character generation).
 *
 * Handles:
 *   - Keyboard layout translation (source layout [default: US English] -> current OS layout)
 *   - Extended key flag inference (for Right Ctrl/Alt, arrow keys, etc.)
 *   - Character generation via ToUnicodeEx
 *   - Key state and auto-repeat tracking
 *
 * Example usage:
 * @code
 *   // Create helper once (tracks key state)
 *   VirtualKeyEventHelper helper;
 *
 *   // When a key down event occurs:
 *   int vk = VK_A;
 *
 *   // Generate and fire KeyEvents (both key down and char events)
 *   for (auto& evt : helper.TranslateKeyEvent(vk, true)) {
 *       view->FireKeyEvent(evt);
 *   }
 *
 *   // Later, when the key is released:
 *   for (auto& evt : helper.TranslateKeyEvent(vk, false)) {
 *       view->FireKeyEvent(evt);
 *   }
 * @endcode
 */
class VirtualKeyEventHelper {
public:
    /**
     * @param source_layout  Keyboard layout used to generate the VK codes.
     *                       Pass nullptr for US English (0x00000409).
     */
    explicit VirtualKeyEventHelper(HKL source_layout = nullptr)
        : source_layout_(source_layout ? source_layout : GetUSEnglishLayout())
    {
    }

    /**
     * Translate a virtual key code to one or more KeyEvents.
     *
     * For key down: returns kType_RawKeyDown (and additional kType_Char events if characters were generated).
     * For key up: returns kType_KeyUp.
     *
     * @param virtual_key  Windows VK_ code
     * @param is_key_down  true for key down, false for key up
     * @return             Vector of KeyEvents (may be empty if translation fails)
     */
    std::vector<KeyEvent> TranslateKeyEvent(int virtual_key, bool is_key_down) {
        std::vector<KeyEvent> result;

        HKL current_layout = GetKeyboardLayout(0);
        bool is_extended = IsExtendedKey(virtual_key);

        // Convert VK -> scancode using source layout
        unsigned int scancode = MapVirtualKeyExW(virtual_key, MAPVK_VK_TO_VSC, source_layout_);
        if (scancode == 0) {
            scancode = MapVirtualKeyW(virtual_key, MAPVK_VK_TO_VSC);
            if (scancode == 0)
                return result;
        }

        // Convert scancode -> VK using current OS layout
        // Skip translation for numpad keys - their scancodes don't distinguish NumLock state
        int target_vk;
        if (IsNumpadKey(virtual_key)) {
            target_vk = virtual_key;
        } else {
            UINT scan_with_extended = is_extended ? (scancode | 0xE000) : scancode;
            target_vk = MapVirtualKeyExW(scan_with_extended, MAPVK_VSC_TO_VK_EX, current_layout);
            if (target_vk == 0)
                target_vk = virtual_key;
        }

        // Track key state for auto-repeat detection
        bool was_down = keys_down_.count(virtual_key) > 0;
        bool is_auto_repeat = is_key_down && was_down;

        if (is_key_down) {
            keys_down_.insert(virtual_key);

            LPARAM lparam = BuildLParam(scancode, is_extended, false, was_down);
            KeyEvent key_down(KeyEvent::kType_RawKeyDown,
                              static_cast<uintptr_t>(target_vk),
                              static_cast<intptr_t>(lparam),
                              false);
            key_down.is_auto_repeat = is_auto_repeat;
            result.push_back(std::move(key_down));

            // Generate character event
            BYTE keyboard_state[256];
            if (GetKeyboardState(keyboard_state)) {
                wchar_t chars[5] = {0};
                int char_count = ToUnicodeEx(target_vk, scancode, keyboard_state,
                                             chars, 4, 0, current_layout);

                for (int i = 0; i < char_count; ++i) {
                    wchar_t c = chars[i];
                    // Allow printable characters and specific control chars (Enter, Tab)
                    if (c >= 0x20 || c == L'\r' || c == L'\n' || c == L'\t') {
                        KeyEvent char_event(KeyEvent::kType_Char,
                                            static_cast<uintptr_t>(c),
                                            static_cast<intptr_t>(lparam),
                                            false);
                        char_event.is_auto_repeat = is_auto_repeat;
                        result.push_back(std::move(char_event));
                    }
                }
            }
        }
        else {
            keys_down_.erase(virtual_key);

            LPARAM lparam = BuildLParam(scancode, is_extended, true, true);
            KeyEvent key_up(KeyEvent::kType_KeyUp,
                            static_cast<uintptr_t>(target_vk),
                            static_cast<intptr_t>(lparam),
                            false);
            result.push_back(std::move(key_up));
        }

        return result;
    }

    /** Clear all tracked key state. */
    void Reset() {
        keys_down_.clear();
    }

private:
    HKL source_layout_;
    std::unordered_set<int> keys_down_;

    static HKL GetUSEnglishLayout() {
        static HKL layout = LoadKeyboardLayoutW(L"00000409", KLF_NOTELLSHELL);
        return layout ? layout : GetKeyboardLayout(0);
    }

    static bool IsExtendedKey(int vk) {
        switch (vk) {
            case VK_RCONTROL:
            case VK_RMENU:
            case VK_LEFT:
            case VK_RIGHT:
            case VK_UP:
            case VK_DOWN:
            case VK_HOME:
            case VK_END:
            case VK_PRIOR:
            case VK_NEXT:
            case VK_INSERT:
            case VK_DELETE:
            case VK_DIVIDE:
            case VK_NUMLOCK:
            case VK_LWIN:
            case VK_RWIN:
            case VK_APPS:
            case VK_SNAPSHOT:
            case VK_CANCEL:
                return true;
            default:
                return false;
        }
    }

    static bool IsNumpadKey(int vk) {
        switch (vk) {
            case VK_NUMPAD0:
            case VK_NUMPAD1:
            case VK_NUMPAD2:
            case VK_NUMPAD3:
            case VK_NUMPAD4:
            case VK_NUMPAD5:
            case VK_NUMPAD6:
            case VK_NUMPAD7:
            case VK_NUMPAD8:
            case VK_NUMPAD9:
            case VK_MULTIPLY:
            case VK_ADD:
            case VK_SEPARATOR:
            case VK_SUBTRACT:
            case VK_DECIMAL:
            case VK_DIVIDE:
                return true;
            default:
                return false;
        }
    }

    static LPARAM BuildLParam(unsigned int scancode, bool is_extended,
                               bool is_key_up, bool was_down) {
        LPARAM lp = 1;                                    // repeat count
        lp |= static_cast<LPARAM>(scancode & 0xFF) << 16; // scancode
        if (is_extended) lp |= (1 << 24);                 // extended flag
        if (was_down)    lp |= (1 << 30);                 // previous key state
        if (is_key_up)   lp |= (1ULL << 31);              // transition state
        return lp;
    }
};

} // namespace ultralight

#endif // _WIN32

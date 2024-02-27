/* This code is an extension of the 'keyboard section' present in 'imgui_demo.cpp'.
   License is the same (MIT License AFAIK)
*/

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif

#include "imgui.h"
#ifndef IMGUI_DISABLE
#include "imgui_internal.h"

#endif

#include "keys.h"


/* This code is an extension of the 'keyboard section' present in 'imgui_demo.cpp'.
   License is the same (MIT License AFAIK)
*/


namespace ImGui {
// VirtualKeyboard Implementation

const char** GetKeyboardLogicalLayoutNames()    {
    static const char* names[] = {"QWERTY","QWERTZ","AZERTY"};
    IM_STATIC_ASSERT(IM_ARRAYSIZE(names)==KLL_COUNT);
    return names;
}
const char** GetKeyboardPhysicalLayoutNames()    {
    static const char* names[] = {"ANSI","ISO","JIS"};
    IM_STATIC_ASSERT(IM_ARRAYSIZE(names)==KPL_COUNT);
    return names;
}
static void ImImplPrivateVirtualKeyboardResetLabels(const char* keyLabels[ImGuiKey_NamedKey_COUNT])   {
    static const char* const KeyLabels[] =  {
    "Tab", "LeftArrow", "RightArrow", "UpArrow", "DownArrow", "PageUp", "PageDown",
    "Home", "End", "Insert", "Delete", "Backspace", "Space", "Enter", "Escape",
    "LeftCtrl", "LeftShift", "LeftAlt", "LeftSuper", "RightCtrl", "RightShift", "RightAlt", "RightSuper", "Menu",
    "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", "G", "H",
    "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
    "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12",
    "F13", "F14", "F15", "F16", "F17", "F18", "F19", "F20", "F21", "F22", "F23", "F24",
    "Apostrophe", "Comma", "Minus", "Period", "Slash", "Semicolon", "Equal", "LeftBracket",
    "Backslash", "RightBracket", "GraveAccent", "CapsLock", "ScrollLock", "NumLock", "PrintScreen",
    "Pause", "Keypad0", "Keypad1", "Keypad2", "Keypad3", "Keypad4", "Keypad5", "Keypad6",
    "Keypad7", "Keypad8", "Keypad9", "KeypadDecimal", "KeypadDivide", "KeypadMultiply",
    "KeypadSubtract", "KeypadAdd", "KeypadEnter", "KeypadEqual",
    "AppBack", "AppForward",
    "GamepadStart", "GamepadBack",
    "GamepadFaceLeft", "GamepadFaceRight", "GamepadFaceUp", "GamepadFaceDown",
    "GamepadDpadLeft", "GamepadDpadRight", "GamepadDpadUp", "GamepadDpadDown",
    "GamepadL1", "GamepadR1", "GamepadL2", "GamepadR2", "GamepadL3", "GamepadR3",
    "GamepadLStickLeft", "GamepadLStickRight", "GamepadLStickUp", "GamepadLStickDown",
    "GamepadRStickLeft", "GamepadRStickRight", "GamepadRStickUp", "GamepadRStickDown",
    "MouseLeft", "MouseRight", "MouseMiddle", "MouseX1", "MouseX2", "MouseWheelX", "MouseWheelY",
    "ModCtrl", "ModShift", "ModAlt", "ModSuper", // ReservedForModXXX are showing the ModXXX names.
    };

    IM_STATIC_ASSERT(ImGuiKey_NamedKey_COUNT == IM_ARRAYSIZE(KeyLabels));
    
    IM_ASSERT(keyLabels);
    memcpy(keyLabels,&KeyLabels[0],ImGuiKey_NamedKey_COUNT*sizeof(char*));
}

ImGuiKey VirtualKeyboard(VirtualKeyboardFlags flags,KeyboardLogicalLayout logicalLayout,KeyboardPhysicalLayout physicalLayout)   {

    IM_ASSERT(logicalLayout!=KLL_COUNT && physicalLayout!=KPL_COUNT);
    const float sf = GetFontSize()/12.f; // scaling factor
    const float  key_rounding = 3.0f;
    const ImVec2 key_face_size = ImVec2(25.0f, 25.0f)*sf;
    const float key_border_width = 10.f*sf;
    const ImVec2 key_size_base = ImVec2(key_face_size.x+key_border_width, key_face_size.y+key_border_width);
    const ImVec2 key_face_pos = ImVec2(5.0f, 3.0f)*sf;
    const float  key_face_rounding = 2.0f;
    const ImVec2 key_label_pos = ImVec2(7.0f, 4.0f)*sf;
    const ImVec2 key_step = ImVec2(key_size_base.x - 1.0f*sf, key_size_base.y - 1.0f*sf);
    const float thickness = 1.f*sf; // used by AddRect(...) and AddLine(...) calls
    const float thicknessDouble = 2.f*thickness;

    ImVec2 board_min = GetCursorScreenPos();
    //ImVec2 board_max = ImVec2(board_min.x + 3 * key_step.x + 2 * key_row_offset + 10.0f, board_min.y + 3 * key_step.y + 10.0f);
    //ImVec2 start_pos = ImVec2(board_min.x + 5.0f - key_step.x, board_min.y);
    ImVec2 start_pos = ImVec2(board_min.x, board_min.y);

    // These should match the 'GKeyNames' variable in  'imgui.cpp', displaying what's written on the keys... most values are unused for now...
    static const char* KeyLabels[ImGuiKey_NamedKey_COUNT] =  {};

    // USAGE:
//#           define IMIMPL_KEY_LABEL(key)  ((key==ImGuiKey_None)?"":KeyLabels[key-ImGuiKey_NamedKey_BEGIN]):
    //IM_ASSERT(key>=ImGuiKey_NamedKey_BEGIN && key<ImGuiKey_NamedKey_END);

    struct KeyLayoutName {ImGuiKey key;float width;};

    static KeyLayoutName keyNamesRows[6][26] ={
        // keyNamesRows[row in 0-6][column in 0-26]

        // rows are trivial: 0 -> F1, F2, etc.;     5 -> Ctrl, Super, Alt, Spacebar, etc.

        // columns:
        //
        //    |---------------------------------|---------------------|-------------------|
        //    0                               17 18                 21 22                 26
        //    |   Base                          |     Arrow           |    Keypad         |

        // ISO Layout
        {{ImGuiKey_Escape,1.75f/1.4f},{ImGuiKey_None,-0.9f/1.2f},{ImGuiKey_F1,1.f},{ImGuiKey_F2,1.f},{ImGuiKey_F3,1.f},{ImGuiKey_F4,1.f},{ImGuiKey_None,-0.52f/1.2f},{ImGuiKey_F5,1.f},{ImGuiKey_F6,1.f},{ImGuiKey_F7,1.f},{ImGuiKey_F8,1.f},{ImGuiKey_None,-0.52f/1.2f},{ImGuiKey_F9,1.f},{ImGuiKey_F10,1.f},{ImGuiKey_F11,1.f},{ImGuiKey_F12,1.f},{ImGuiKey_None,0.f},    {ImGuiKey_None,-0.52f},    {ImGuiKey_PrintScreen,1.f},{ImGuiKey_ScrollLock,1.f},{ImGuiKey_Pause,1.f},    {ImGuiKey_None,0.f},    {ImGuiKey_None,0.f},{ImGuiKey_None,0.f},{ImGuiKey_None,0.f}             },
        {{ImGuiKey_None,1.f},{ImGuiKey_1,1.f},{ImGuiKey_2,1.f},{ImGuiKey_3,1.f},{ImGuiKey_4,1.f},{ImGuiKey_5,1.f},{ImGuiKey_6,1.f},{ImGuiKey_7,1.f},{ImGuiKey_8,1.f},{ImGuiKey_9,1.f},{ImGuiKey_0,1.f},{ImGuiKey_None,1.f},{ImGuiKey_None,1.f},{ImGuiKey_None,0.f},{ImGuiKey_Backspace,3.25f/1.2f},{ImGuiKey_None,0.f},{ImGuiKey_None,0.f},    {ImGuiKey_None,-0.52f},          {ImGuiKey_Insert,1.f},{ImGuiKey_Home,1.f},{ImGuiKey_PageUp,1.f},    {ImGuiKey_None,-0.52f/1.2f},    {ImGuiKey_NumLock,1.f},{ImGuiKey_KeypadDivide,1.f},{ImGuiKey_KeypadMultiply,1.f},{ImGuiKey_KeypadSubtract,1.f}      },
        {{ImGuiKey_Tab,2.15f/1.2f},{ImGuiKey_Q,1.f},{ImGuiKey_W,1.f},{ImGuiKey_E,1.f},{ImGuiKey_R,1.f},{ImGuiKey_T,1.f},{ImGuiKey_Y,1.f},{ImGuiKey_U,1.f},{ImGuiKey_I,1.f},{ImGuiKey_O,1.f},{ImGuiKey_P,1.f},{ImGuiKey_None,1.f},{ImGuiKey_None,1.f},{ImGuiKey_Enter,2.3f/1.2f},{ImGuiKey_None,0.f},{ImGuiKey_None,0.f},{ImGuiKey_None,0.f},    {ImGuiKey_None,-0.52f},         {ImGuiKey_Delete,1.f},{ImGuiKey_End,1.f},{ImGuiKey_PageDown,1.f},    {ImGuiKey_None,-0.52f/1.2f},    {ImGuiKey_Keypad7,1.f},{ImGuiKey_Keypad8,1.f},{ImGuiKey_Keypad9,1.f},{ImGuiKey_KeypadAdd,1.f}      },
        {{ImGuiKey_CapsLock,2.7f/1.2f},{ImGuiKey_A,1.f},{ImGuiKey_S,1.f},{ImGuiKey_D,1.f},{ImGuiKey_F,1.f},{ImGuiKey_G,1.f},{ImGuiKey_H,1.f},{ImGuiKey_J,1.f},{ImGuiKey_K,1.f},{ImGuiKey_L,1.f},{ImGuiKey_None,1.f},{ImGuiKey_None,1.f},{ImGuiKey_None,1.f},{ImGuiKey_Enter,1.74f/1.2f},{ImGuiKey_None,0.f},{ImGuiKey_None,0.f},{ImGuiKey_None,0.f},    {ImGuiKey_None,-0.52f}, {ImGuiKey_None,-1.f},{ImGuiKey_None,-1.f},{ImGuiKey_None,-1.f},    {ImGuiKey_None,-0.52f/1.2f},    {ImGuiKey_Keypad4,1.f},{ImGuiKey_Keypad5,1.f},{ImGuiKey_Keypad6,1.f},{ImGuiKey_KeypadAdd,1.f}                    },
        {{ImGuiKey_LeftShift,1.74f/1.2f},{ImGuiKey_None,1.f},{ImGuiKey_Z,1.f},{ImGuiKey_X,1.f},{ImGuiKey_C,1.f},{ImGuiKey_V,1.f},{ImGuiKey_B,1.f},{ImGuiKey_N,1.f},{ImGuiKey_M,1.f},{ImGuiKey_None,1.f},{ImGuiKey_None,1.f},{ImGuiKey_None,1.f},{ImGuiKey_None,0.f},{ImGuiKey_RightShift,4.4f/1.2f},{ImGuiKey_None,0.f},{ImGuiKey_None,0.f},{ImGuiKey_None,0.f},    {ImGuiKey_None,-0.6f/1.2f},    {ImGuiKey_None,-1.f},{ImGuiKey_UpArrow,1.f},{ImGuiKey_None,-1.f},    {ImGuiKey_None,-0.52f/1.2f},    {ImGuiKey_Keypad1,1.f},{ImGuiKey_Keypad2,1.f},{ImGuiKey_Keypad3,1.f},{ImGuiKey_KeypadEnter,1.f}                    },
        {{ImGuiKey_LeftCtrl,1.74f/1.2f},{ImGuiKey_LeftSuper,1.74f/1.2f},{ImGuiKey_LeftAlt,1.74f/1.2f},{ImGuiKey_None,0.f},{ImGuiKey_Space,9.6f/1.2f},{ImGuiKey_None,0.f},{ImGuiKey_None,0.f},{ImGuiKey_RightAlt,1.74f/1.2f},{ImGuiKey_RightSuper,1.74f/1.2f},{ImGuiKey_Menu,1.74f/1.2f},{ImGuiKey_RightCtrl,1.74f/1.2f},{ImGuiKey_None,0.f},{ImGuiKey_None,0.f},{ImGuiKey_None,0.f},{ImGuiKey_None,0.f},{ImGuiKey_None,0.f},{ImGuiKey_None,0.f},    {ImGuiKey_None,-0.52f/1.2f},    {ImGuiKey_LeftArrow,1.f},{ImGuiKey_DownArrow,1.f},{ImGuiKey_RightArrow,1.f},    {ImGuiKey_None,-0.52f/1.2f},    {ImGuiKey_Keypad0,2.9f/1.2f},{ImGuiKey_None,0.f},{ImGuiKey_KeypadDecimal,1.f},{ImGuiKey_KeypadEnter,1.f}                    }
    };

    static KeyboardPhysicalLayout phyLayout = KPL_COUNT;  // KPL_ISO, but it's just for initializing stuff
    static KeyboardLogicalLayout logLayout = KLL_QWERTY;

    if (phyLayout==KPL_COUNT)    {
        phyLayout = KPL_ISO;
        ImImplPrivateVirtualKeyboardResetLabels(KeyLabels);

        // well, we should do this every frame (or store a reference of the last checked font),
        // but we just do it once at startup
        static const char* utf8[] = {
            "\xe2\x87\xa7",  // 0x21E7 ⇧
            "\xe2\xac\x86",  // 0x2B06 ⬆
            "\xe2\x86\x91",  // 0x2191 ↑
            "\xe2\x9f\xb5",  // 0x27F5 ⟵
            "\xe2\x86\x90",  // 0x2190 ←
            "\xe2\x86\x96",  // 0x2196 ↖
            "\xe2\x8c\x82",  // 0x2302 ⌂
            "\xe2\x8f\x8f",  // 0x23CF ⏏
            "\xe2\x86\xb9",  // 0x21B9 ↹
            "\xe2\x87\xa4\n\xe2\x87\xa5",  // 0x21E4,0x21E5 ⇤⇥
            "\xe2\x86\x92",  // 0x2191 →
            "\xe2\x86\x93",  // 0x2191 ↓
            "Pg\xe2\x86\x91",  // 0x2191 Pg↑
            "Pg\xe2\x86\x93",  // 0x2193 Pg↓
            "\xe2\x88\x97",     // 0x2217 ∗
            "\xe2\x88\x92",     // 0x2212 −
            "\xe2\x8f\xb8",     // 0x23f8 ⏸
            "\xe2\x96\xae\xe2\x96\xae",     // 0x25AE ▮▮
            "\xe2\x96\xae\xe2\x96\xaf",     // 0x25AF ▯▯
            "Enter\n\xe2\x86\xb5",    // 0x21B5 ↵
            "Enter\n\xe2\x86\xa9",    // 0x21A9 ↩
            "Enter\n\xe2\xa4\xb6"   // 0x2936 ⤶
        };

        const ImFont* font = ImGui::GetFont();
#       define IMIMPL_CHANGE_GLYPH(K,C,S)     if (font->FindGlyphNoFallback((C)))  KeyLabels[(K)-ImGuiKey_NamedKey_BEGIN]=S
#       define IMIMPL_CHANGE_GLYPH_FALLBACK1(K,C,S,C1,S1)     if (font->FindGlyphNoFallback((C)))  KeyLabels[(K)-ImGuiKey_NamedKey_BEGIN]=S;  \
                                                            else if (font->FindGlyphNoFallback((C1)))  KeyLabels[(K)-ImGuiKey_NamedKey_BEGIN]=S1
#       define IMIMPL_CHANGE_GLYPH_FALLBACK2(K,C,S,C1,S1,C2,S2)     if (font->FindGlyphNoFallback((C)))  KeyLabels[(K)-ImGuiKey_NamedKey_BEGIN]=S;  \
                                                                    else if (font->FindGlyphNoFallback((C1)))  KeyLabels[(K)-ImGuiKey_NamedKey_BEGIN]=S1;   \
                                                                    else if (font->FindGlyphNoFallback((C2)))  KeyLabels[(K)-ImGuiKey_NamedKey_BEGIN]=S2
#       define IMIMPL_CHANGE_GLYPH_ARROWS(KL,KU,KR,KD,CL,CU,CR,CD,SL,SU,SR,SD)     if (font->FindGlyphNoFallback((CL)) && font->FindGlyphNoFallback((CU)) && font->FindGlyphNoFallback((CR)) && font->FindGlyphNoFallback((CD))) \
                                                                                        {KeyLabels[(KL)-ImGuiKey_NamedKey_BEGIN]=SL;KeyLabels[(KU)-ImGuiKey_NamedKey_BEGIN]=SU;KeyLabels[(KR)-ImGuiKey_NamedKey_BEGIN]=SR;KeyLabels[(KD)-ImGuiKey_NamedKey_BEGIN]=SD;}

        IMIMPL_CHANGE_GLYPH_FALLBACK2(ImGuiKey_LeftShift, 0x21E7,utf8[0],0x2B06,utf8[1],0x2191,utf8[2]);
        IMIMPL_CHANGE_GLYPH_FALLBACK2(ImGuiKey_RightShift, 0x21E7,utf8[0],0x2B06,utf8[1],0x2191,utf8[2]);
        IMIMPL_CHANGE_GLYPH_FALLBACK1(ImGuiKey_Backspace,0x27F5,utf8[3],0x2190,utf8[4]);
        IMIMPL_CHANGE_GLYPH_FALLBACK1(ImGuiKey_Home, 0x2196,utf8[5],0x2302,utf8[6]);
        IMIMPL_CHANGE_GLYPH(ImGuiKey_CapsLock, 0x23CF,utf8[7]);
        IMIMPL_CHANGE_GLYPH(ImGuiKey_Tab, 0x21B9,utf8[8]);
        else if (font->FindGlyphNoFallback(0x21E4) && font->FindGlyphNoFallback(0x21E5)) KeyLabels[ImGuiKey_Tab-ImGuiKey_NamedKey_BEGIN]=utf8[9];
        IMIMPL_CHANGE_GLYPH_ARROWS(ImGuiKey_LeftArrow,ImGuiKey_UpArrow,ImGuiKey_RightArrow,ImGuiKey_DownArrow,
                                   0x2190,0x2191,0x2192,0x2193,
                                   utf8[4],utf8[5],utf8[10],utf8[11])        // no semicolon here
        IMIMPL_CHANGE_GLYPH_ARROWS(ImGuiKey_GamepadDpadLeft,ImGuiKey_GamepadDpadUp,ImGuiKey_GamepadDpadRight,ImGuiKey_GamepadDpadDown,
                                   0x2190,0x2191,0x2192,0x2193,
                                           utf8[4],utf8[5],utf8[10],utf8[11])        // no semicolon here
        IMIMPL_CHANGE_GLYPH_ARROWS(ImGuiKey_GamepadLStickLeft,ImGuiKey_GamepadLStickUp,ImGuiKey_GamepadLStickRight,ImGuiKey_GamepadLStickDown,
                                   0x2190,0x2191,0x2192,0x2193,
                                   utf8[4],utf8[5],utf8[10],utf8[11])        // no semicolon here
        IMIMPL_CHANGE_GLYPH_ARROWS(ImGuiKey_GamepadRStickLeft,ImGuiKey_GamepadRStickUp,ImGuiKey_GamepadRStickRight,ImGuiKey_GamepadRStickDown,
                                   0x2190,0x2191,0x2192,0x2193,
                                   utf8[4],utf8[5],utf8[10],utf8[11])        // no semicolon here
        IMIMPL_CHANGE_GLYPH_ARROWS(ImGuiKey_GamepadFaceLeft,ImGuiKey_GamepadFaceUp,ImGuiKey_GamepadFaceRight,ImGuiKey_GamepadFaceDown,
                                   0x2190,0x2191,0x2192,0x2193,
                                    utf8[4],utf8[5],utf8[10],utf8[11])        // no semicolon here
        IMIMPL_CHANGE_GLYPH(ImGuiKey_PageUp, 0x2191,utf8[12]);
        IMIMPL_CHANGE_GLYPH(ImGuiKey_PageDown, 0x2193,utf8[13]);
        IMIMPL_CHANGE_GLYPH(ImGuiKey_KeypadMultiply, 0x2217,utf8[14]);
        IMIMPL_CHANGE_GLYPH(ImGuiKey_KeypadSubtract, 0x2212,utf8[15]);
        IMIMPL_CHANGE_GLYPH_FALLBACK2(ImGuiKey_Pause, 0x23f8,utf8[16],0x25AE,utf8[17],0x25AF,utf8[18]);
        IMIMPL_CHANGE_GLYPH_FALLBACK2(ImGuiKey_Enter, 0x21B5,utf8[19],0x21A9,utf8[20],0x2936,utf8[21]);


#       undef IMIMPL_CHANGE_GLYPH
#       undef IMIMPL_CHANGE_GLYPH_FALLBACK1
#       undef IMIMPL_CHANGE_GLYPH_FALLBACK2
#       undef IMIMPL_CHANGE_GLYPH_ARROWS
    }

    IM_ASSERT(phyLayout!=KPL_COUNT && logLayout!=KLL_COUNT);

    if (phyLayout!=physicalLayout)  {
        // we must update the physical layout
        phyLayout = physicalLayout;
        KeyLayoutName *R1 = keyNamesRows[1], *R2 = keyNamesRows[2], *R3 = keyNamesRows[3], *R4 = keyNamesRows[4], *R5 = keyNamesRows[5];
        IM_ASSERT(R1[14].key==ImGuiKey_Backspace);
        IM_ASSERT(R2[13].key==ImGuiKey_Enter || R2[13].key==ImGuiKey_Backslash);
        IM_ASSERT(R3[13].key==ImGuiKey_Enter);
        IM_ASSERT(R4[13].key==ImGuiKey_RightShift);
        IM_ASSERT(R5[4].key==ImGuiKey_Space && R5[7].key==ImGuiKey_RightAlt && R5[10].key==ImGuiKey_RightCtrl);
        switch (phyLayout)  {
        case KPL_ANSI:   {
            R1[13].width=0.f;R1[14].width=3.25f/1.2f;
            R2[13].key=ImGuiKey_Backslash;
            R3[12].width=0.f;R3[13].width=3.425f/1.2f;
            R4[0].width=3.45f/1.2f; R4[1].width=0.f;R4[12].width=0.f;R4[13].width=4.4f/1.2f;//R4[12].width=1.f;R4[13].width=2.725f/1.2f;
            R5[3].width=0.f;R5[4].width=9.6f/1.2f;R5[5].width=0.f;R5[6].width=0.f;R5[7].width=1.74f/1.2f;R5[8].width=1.74f/1.2f;R5[9].width=1.74f/1.2f;R5[10].width=1.74f/1.2f;
        }
            break;
        case KPL_ISO:    {
            R1[13].width=0.f;R1[14].width=3.25f/1.2f;
            R2[13].key=ImGuiKey_Enter;
            R3[12].width=1.f;R3[13].width=1.74f/1.2f;
            R4[0].width=1.74f/1.2f; R4[1].width=1.f;R4[12].width=0.f;R4[13].width=4.4f/1.2f;
            R5[3].width=0.f;R5[4].width=9.6f/1.2f;R5[5].width=0.f;R5[6].width=0.f;R5[7].width=1.74f/1.2f;R5[8].width=1.74f/1.2f;R5[9].width=1.74f/1.2f;R5[10].width=1.74f/1.2f;
        }
            break;
        case KPL_JIS:    {
            R1[13].width=1.f;R1[14].width=1.575f/1.2f;
            R2[13].key=ImGuiKey_Enter;
            R3[12].width=1.f;R3[13].width=1.74f/1.2f;
            R4[0].width=3.45f/1.2f; R4[1].width=0.f;R4[12].width=1.f;R4[13].width=2.725f/1.2f;
            R5[3].width=0.98f;R5[4].width=6.1f/1.2f;R5[5].width=0.98f;R5[6].width=0.98f;R5[7].width=1.38f/1.2f;R5[8].width=1.38f/1.2f;R5[9].width=1.38f/1.2f;R5[10].width=1.38f/1.2f;
        }
            break;
        default:
            IM_ASSERT(0);
            break;

        }
    }
    if (logLayout!=logicalLayout)    {
        // we must update the keyboard layout
        logLayout = logicalLayout;
        KeyLayoutName *R2 = keyNamesRows[2], *R3 = keyNamesRows[3], *R4 = keyNamesRows[4];
        switch (logLayout)  {
        case KLL_QWERTY:  {
            // QWERTY
            R2[1].key=ImGuiKey_Q;R2[2].key=ImGuiKey_W;R2[6].key=ImGuiKey_Y;
            R3[1].key=ImGuiKey_A;R3[10].key=ImGuiKey_None;
            R4[2].key=ImGuiKey_Z;R4[8].key=ImGuiKey_M;
        }
            break;
        case KLL_QWERTZ:  {
            // QWERTZ
            R2[1].key=ImGuiKey_Q;R2[2].key=ImGuiKey_W;R2[6].key=ImGuiKey_Z;
            R3[1].key=ImGuiKey_A;R3[10].key=ImGuiKey_None;
            R4[2].key=ImGuiKey_Y;R4[8].key=ImGuiKey_M;
        }
            break;
        case KLL_AZERTY: {
            // AZERTY
            R2[1].key=ImGuiKey_A;R2[2].key=ImGuiKey_Z;R2[6].key=ImGuiKey_Y;
            R3[1].key=ImGuiKey_Q;R3[10].key=ImGuiKey_M;
            R4[2].key=ImGuiKey_W;R4[8].key=ImGuiKey_None;
        }
            break;
        default:
            IM_ASSERT(0);
            break;
        }
    }

    ImVec2 cur_pos = start_pos;ImVec2 board_max = start_pos;
    ImDrawList* draw_list = GetWindowDrawList();

    int rStart = 0,rEnd = 6;
    int cStart = 0,cEnd = 26;

    if (flags&VirtualKeyboardFlags_ShowBaseBlock && flags&VirtualKeyboardFlags_ShowKeypadBlock) flags|=VirtualKeyboardFlags_ShowArrowBlock;
    if (!(flags&VirtualKeyboardFlags_ShowFunctionBlock)) rStart=1;
    if (flags&VirtualKeyboardFlags_ShowFunctionBlock && !(flags&(VirtualKeyboardFlags_ShowBaseBlock|VirtualKeyboardFlags_ShowArrowBlock|VirtualKeyboardFlags_ShowKeypadBlock))) rEnd=1;
    if (!(flags&(VirtualKeyboardFlags_ShowArrowBlock|VirtualKeyboardFlags_ShowKeypadBlock))) cEnd=17;
    else if (!(flags&(VirtualKeyboardFlags_ShowKeypadBlock))) cEnd=21;
    if ((flags&VirtualKeyboardFlags_ShowAllBlocks)==VirtualKeyboardFlags_ShowFunctionBlock)  {
       // user wants only the function block: we don't give him the part above the arrow block
       rStart=0;rEnd=1;cStart=0;cEnd=17;
    }
    else if (!(flags&VirtualKeyboardFlags_ShowBaseBlock))  {
        if (flags&VirtualKeyboardFlags_ShowArrowBlock) cStart=18;
        else {
            cStart=22;
            rStart=1;flags&=~VirtualKeyboardFlags_ShowFunctionBlock;   // optional line: removes empty space above Keypad when shown alone.
        }
    }

    const bool mouseEnabled = !(flags&VirtualKeyboardFlags_NoMouseInteraction);
    const bool keyboardEnabled = !(flags&VirtualKeyboardFlags_NoKeyboardInteraction);
    const bool mouseDown = ImGui::IsMouseDown(ImGuiMouseButton_Left);
    ImGuiKey mouseClickedKey = ImGuiKey_COUNT;    
    ImGuiKey keyboardClickedKey = ImGuiKey_COUNT;

    ImVec2 key_min_upper_enter(0,0),key_max_upper_enter(0,0);
    ImVec2 key_min_lower_enter(0,0),key_max_lower_enter(0,0);

    //draw_list->PushClipRect(board_min, board_max, true);
    for (int r = rStart; r < rEnd; r++) {
        cur_pos.x = start_pos.x;
        if (flags&VirtualKeyboardFlags_ShowFunctionBlock) {
            cur_pos.y = start_pos.y+r*key_step.y+(r>0?(key_face_size.x*0.7f/1.2f):0.f);
        }
        else {
            IM_ASSERT(r!=0);
            cur_pos.y = start_pos.y+(r-1)*key_step.y;
        }
        const KeyLayoutName *keyDataRow = keyNamesRows[r];
        for (int c = cStart; c < cEnd; c++) {
            const KeyLayoutName* key_data = &keyDataRow[c];

            if (key_data->width==0.f) continue;
            if (key_data->width<0.f) {
                // spacing entry
                IM_ASSERT(key_data->key==ImGuiKey_None);
                cur_pos.x+=key_face_size.x*(-key_data->width)+key_border_width;
                continue;
            }
            IM_ASSERT(key_data->key==ImGuiKey_None || (key_data->key>=ImGuiKey_NamedKey_BEGIN && key_data->key<ImGuiKey_NamedKey_END));
            const char* key_label = (key_data->key==ImGuiKey_None)?"":KeyLabels[key_data->key-ImGuiKey_NamedKey_BEGIN];

            // Draw the key ----------------------------------------------------------------------------
            ImDrawFlags df = ImDrawFlags_RoundCornersAll;

            ImVec2 key_min = ImVec2(cur_pos.x, cur_pos.y);
            ImVec2 key_max = ImVec2(key_min.x + key_face_size.x*key_data->width+key_border_width, key_min.y + key_size_base.y);
            ImVec2 face_min = ImVec2(key_min.x + key_face_pos.x, key_min.y + key_face_pos.y);
            ImVec2 face_max = ImVec2(face_min.x + key_face_size.x*key_data->width, face_min.y + key_face_size.y);
            ImVec2 label_min = ImVec2(key_min.x + key_label_pos.x, key_min.y + key_label_pos.y);

            if (key_data->key==ImGuiKey_Enter && phyLayout!=KPL_ANSI)  {
                if (r==2)   {
                    df = ImDrawFlags_RoundCornersTop|ImDrawFlags_RoundCornersBottomLeft;
                    key_min_upper_enter = key_min;key_max_upper_enter = key_max;
                }
                else if (r==3)  {
                    df = ImDrawFlags_RoundCornersBottom;key_label="";
                    key_min_lower_enter = key_min; key_max_lower_enter = key_max;
                    key_min_lower_enter.y = key_max_upper_enter.y;
                    key_min.y = cur_pos.y-6.f*sf;  // The last number will be reused twice
                    face_min.y = key_min.y;
                }
                else {IM_ASSERT(0);}
            }
            else if (c==25) {//key_data->key==ImGuiKey_KeypadAdd || key_data->key==ImGuiKey_KeypadEnter)
                if (r==2 || r==4) continue;
                if (r==3 || r==5)   {
                    // This button requires two rows: draw the whole button at once
                    if (flags&VirtualKeyboardFlags_ShowFunctionBlock) key_min.y = start_pos.y+(r-1)*key_step.y+key_face_size.x*0.7f/1.2f;
                    else key_min.y = start_pos.y+(r-2)*key_step.y;
                    face_min.y = key_min.y + key_face_pos.y;
                    label_min.y = key_min.y + key_label_pos.y;
                }
            }

            draw_list->AddRectFilled(key_min, key_max, IM_COL32(204, 204, 204, 255), key_rounding, df);
            if (key_data->key!=ImGuiKey_Enter || r==2)  {
                draw_list->AddRect(key_min, key_max, IM_COL32(24, 24, 24, 255), key_rounding, df, thickness);
                draw_list->AddRect(face_min, face_max, IM_COL32(193, 193, 193, 255), key_face_rounding, ImDrawFlags_None, thicknessDouble);
            }
            else  {
                IM_ASSERT(r==3);
                draw_list->AddLine(ImVec2(key_min.x,key_min.y+6.f*sf), ImVec2(key_min.x,key_max.y), IM_COL32(24, 24, 24, 255), thickness); // left
                draw_list->AddLine(ImVec2(key_min.x,key_max.y), key_max, IM_COL32(24, 24, 24, 255), thickness); // bottom
                draw_list->AddLine(ImVec2(key_max.x,key_min.y-1.f), key_max, IM_COL32(24, 24, 24, 255), thickness); // right
                draw_list->AddLine(ImVec2(face_min.x,face_min.y+6.f*sf), ImVec2(face_min.x,face_max.y), IM_COL32(193, 193, 193, 255), thicknessDouble); // left
                draw_list->AddLine(ImVec2(face_min.x,face_max.y), face_max, IM_COL32(193, 193, 193, 255), thicknessDouble);   // bottom
                draw_list->AddLine(ImVec2(face_max.x,face_min.y), face_max, IM_COL32(193, 193, 193, 255), thicknessDouble);   // right
            }
            draw_list->AddRectFilled(face_min, face_max, IM_COL32(252, 252, 252, 255), key_face_rounding, df);
            draw_list->AddText(label_min, IM_COL32(64, 64, 64, 255), key_label);

            // Process input and draw the 'mouse hovered' and 'pressed' color----------------------------------
            if (key_data->key!=ImGuiKey_Enter || phyLayout==KPL_ANSI)  {
                if (keyboardEnabled && key_data->key!=ImGuiKey_None)    {
                    if (ImGui::IsKeyDown(key_data->key)) {
                        draw_list->AddRectFilled(key_min, key_max, IM_COL32(255, 0, 0, 128), key_rounding, df);
                    }
                    if ((keyboardClickedKey==ImGuiKey_COUNT || keyboardClickedKey==ImGuiKey_None) && ImGui::IsKeyReleased(key_data->key)) keyboardClickedKey = key_data->key;
                }
                if (mouseEnabled)   {
                    if (ImGui::IsMouseHoveringRect(key_min,key_max))    {
                        const ImU32 color = mouseDown ? IM_COL32(255, 0, 0, 128) : IM_COL32(0, 0, 0, 32);
                        if (mouseDown)  mouseClickedKey = key_data->key;
                        draw_list->AddRectFilled(key_min, key_max, color, key_rounding, df);
                    }
                }
            }
            else if (r==3)  {
                IM_ASSERT(key_data->key==ImGuiKey_Enter);
                if (keyboardEnabled && key_data->key!=ImGuiKey_None)    {
                    if (ImGui::IsKeyDown(key_data->key)) {
                        draw_list->AddRectFilled(key_min_upper_enter, key_max_upper_enter, IM_COL32(255, 0, 0, 128), key_rounding, ImDrawFlags_RoundCornersTop|ImDrawFlags_RoundCornersBottomLeft);
                        draw_list->AddRectFilled(key_min_lower_enter, key_max_lower_enter, IM_COL32(255, 0, 0, 128), key_rounding, ImDrawFlags_RoundCornersBottom);
                    }
                    if ((keyboardClickedKey==ImGuiKey_COUNT || keyboardClickedKey==ImGuiKey_None) && ImGui::IsKeyReleased(key_data->key)) keyboardClickedKey = key_data->key;
                }
                if (mouseEnabled)   {
                    if (ImGui::IsMouseHoveringRect(key_min_upper_enter,key_max_upper_enter) || ImGui::IsMouseHoveringRect(key_min_lower_enter,key_max_lower_enter))    {
                        const ImU32 color = mouseDown ? IM_COL32(255, 0, 0, 128) : IM_COL32(0, 0, 0, 32);
                        if (mouseDown)  mouseClickedKey = key_data->key;
                        draw_list->AddRectFilled(key_min_upper_enter, key_max_upper_enter, color, key_rounding, ImDrawFlags_RoundCornersTop|ImDrawFlags_RoundCornersBottomLeft);
                        draw_list->AddRectFilled(key_min_lower_enter, key_max_lower_enter, color, key_rounding, ImDrawFlags_RoundCornersBottom);
                    }
                }
            }
            //----------------------------------------------------------------------------------------------------------------
            cur_pos.x = key_max.x;
            board_max.y = key_max.y;
        }
        board_max.x = cur_pos.x;
    }
    //draw_list->PopClipRect();
    ImGui::Dummy(ImVec2(board_max.x - board_min.x, board_max.y - board_min.y));
    if (mouseEnabled)   {
        if (!ImGui::IsItemClicked(ImGuiMouseButton_Left)) mouseClickedKey = ImGuiKey_COUNT;
        else {printf("keyClicked: %s\n",mouseClickedKey==ImGuiKey_None?"None":ImGui::GetKeyName(mouseClickedKey));fflush(stdout);}
    }
    if (keyboardClickedKey!=ImGuiKey_COUNT)    {
        if (mouseClickedKey==ImGuiKey_COUNT || mouseClickedKey==ImGuiKey_None) return keyboardClickedKey;
        else return mouseClickedKey;
    }
    else return mouseClickedKey;
}

} // namespace ImGui

namespace ImGui {
    // Virtual Keyboard
    // USAGE: to get started, just call VirtualKeyboard() in one of your ImGui windows
    enum KeyboardLogicalLayout {
      KLL_QWERTY = 0,
      KLL_QWERTZ,
      KLL_AZERTY,
      KLL_COUNT
    };
    IMGUI_API const char** GetKeyboardLogicalLayoutNames();
    enum KeyboardPhysicalLayout {
      KPL_ANSI = 0,
      KPL_ISO,
      KPL_JIS,
      KPL_COUNT
    };
    IMGUI_API const char** GetKeyboardPhysicalLayoutNames();
    enum VirtualKeyboardFlags_ {
        VirtualKeyboardFlags_ShowBaseBlock       =   1<<0,
        VirtualKeyboardFlags_ShowFunctionBlock   =   1<<1,
        VirtualKeyboardFlags_ShowArrowBlock      =   1<<2,  // This can't be excluded when both VirtualKeyboardFlags_BlockBase and VirtualKeyboardFlags_BlockKeypad are used
        VirtualKeyboardFlags_ShowKeypadBlock     =   1<<3,
        VirtualKeyboardFlags_ShowAllBlocks        =   VirtualKeyboardFlags_ShowBaseBlock|VirtualKeyboardFlags_ShowFunctionBlock|VirtualKeyboardFlags_ShowArrowBlock|VirtualKeyboardFlags_ShowKeypadBlock,
        VirtualKeyboardFlags_NoMouseInteraction = 1<<4,
        VirtualKeyboardFlags_NoKeyboardInteraction = 1<<5,
        VirtualKeyboardFlags_NoInteraction = VirtualKeyboardFlags_NoMouseInteraction | VirtualKeyboardFlags_NoKeyboardInteraction
    };
    typedef int VirtualKeyboardFlags;
    // Displays a virtual keyboard.
    // It always returns ImGuiKey_COUNT, unless:
    //     a) a mouse is clicked (clicked event) on a key AND flag VirtualKeyboardFlags_NoMouseInteraction is not used (DEFAULT)
    //     b) a key is typed (released event) AND VirtualKeyboardFlags_NoKeyboardInteraction is not used (DEFAULT). Note that multiple keys can be pressed together, but only one is returned.
    // In that case, it returns the clicked (or typed) key.
    IMGUI_API ImGuiKey VirtualKeyboard(VirtualKeyboardFlags flags=VirtualKeyboardFlags_ShowAllBlocks,KeyboardLogicalLayout logicalLayout=KLL_QWERTY,KeyboardPhysicalLayout physicalLayout=KPL_ISO);

    // Possible improvements (in random order):
    // 1) The L-shaped enter key is not displayed perfectly. Improve it.
    // 2) Add entries to the KeyboardLogicalLayout enum and implement keyboards for specific countries, riducing the number of 'empty' keys present in the general layout.
}
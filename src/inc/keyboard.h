#ifndef KEYBOARD_H
# define KEYBOARD_H

unsigned char keyboard_map[128] = {
  // -------- 0 to 9 --------
  0,
  27, // escape key
  '1','2','3','4','5','6','7','8','9','0',
  // -------- 10 to 19 --------
  '-','=',
  '\b', // Backspace
  '\t', // Tab
  'q','w','e','r',
  // -------- 20 to 29 --------
  't','y','u','i','o','p','[',']',
  '\n', // Enter
  -11, // left Ctrl
  // -------- 30 to 39 --------
  'a','s','d','f','g','h','j','k','l',';',
  // -------- 40 to 49 --------
  '\'','`',
  -12, // left Shift
  '\\','z','x','c','v','b','n',
  // -------- 50 to 59 --------
  'm',',','.','/',
  0, // right Shift
  '*', // numpad asterisk
  0, // left Alt
  ' ', // Spacebar
  0,
  -1, // F1
  // -------- 60 to 69 --------
  -2, // F2
  -3, // F3
  -4, // F4
  -5, // F5
  -6, // F6
  -7, // F7
  -8, // F8
  -9, // F9
  -10, // F10
  0,
  // -------- 70 to 79 --------
  0, // scroll lock
  '7', // numpad 7, HOME key if preceded by keycode 224
  0, // numpad 8, up arrow if preceded by keycode 224
  '9', // numpad 9, PAGE UP key if preceded by keycode 224
  '-', // numpad hyphen
  -13, // numpad 4, left arrow if preceded by keycode 224
  '5', // numpad 5
  -14, // numpad 6, right arrow if preceded by keycode 224
  '+',
  '1', // numpad 1, END key if preceded by keycode 224
  // -------- 80 to 89 --------
  0, // numpad 2, down arrow if preceded by keycode 224
  '3', // numpad 3, PAGE DOWN key if preceded by keycode 224
  '0', // numpad 0, INSERT key if preceded by keycode 224
  '.', // numpad dot, DELETE key if preceded by keycode 224
  0,0,0,0,0,0,
  // -------- 90 to 99 --------
  0,0,0,0,0,0,0,0,0,0,
  // -------- 100 to 109 --------
  0,0,0,0,0,0,0,0,0,0,
  // -------- 110 to 119 --------
  0,0,0,0,0,0,0,0,0,0,
  // -------- 120-127 --------
  0,0,0,0,0,0,0,0,
};

#endif
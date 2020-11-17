#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <optional>
#include "cartridge.cpp"
#include "cpu.cpp"
#include "io_device.cpp"
#include "joypad.cpp"
#include "mmu.cpp"
#include "ppu.cpp"
#include "timer.cpp"

cpu cpu;

// Translates keycode to `joypad::Key` enum.
//std::optional<joypad::Key> translate_keycode(SDL_Keycode key){
joypad::Key translate_keycode(SDL_Keycode key){
    
    joypad::Key temp;

    switch(key){
        case SDLK_DOWN:
            temp = joypad::Key::Down;
            break;
        case SDLK_UP:
            temp = joypad::Key::Up;
            break;
        case SDLK_LEFT:
            temp = joypad::Key::Left;
            break;
        case SDLK_RIGHT:
            temp = joypad::Key::Right;
            break;
        case SDLK_RETURN:
            temp = joypad::Key::Start;
            break;
        case SDLK_SPACE:
            temp = joypad::Key::Select;
            break;
        case SDLK_z:
            temp = joypad::Key::A;
            break;
        case SDLK_x:
            temp = joypad::Key::B;
            break;
        default:
            break;
    }

    return temp;
}

// Handles key down event.
void handle_keydown(SDL_Keycode key){
    joypad.keydown(translate_keycode(key));
}

// Handles key up event.
void handle_keyup(SDL_Keycode key){
    joypad.keyup(translate_keycode(key));
}

int main(int argc, char* argv[]) {

    SDL_Window *window;                    // ポインタを宣言する

    SDL_Init(SDL_INIT_VIDEO);              // SDL2を初期化する

    // 次の設定でアプリケーションウィンドウを生成する:
    window = SDL_CreateWindow(
        "CameBoyz SDL2 window",             // ウィンドウのタイトル
        SDL_WINDOWPOS_UNDEFINED,           // X座標の初期値
        SDL_WINDOWPOS_UNDEFINED,           // Y座標の初期値
        320,                               // 幅のピクセル数
        288,                               // 高さのピクセル数
        SDL_WINDOW_OPENGL                  // フラグ
    );

    // ウィンドウの生成に成功したかチェックする
    if (window == NULL) {
        // ここを通ったならばウィンドウを生成できなかった...
        printf("ウィンドウを生成できなかった: %s\n", SDL_GetError());
        return 1;
    }

    // ウィンドウが開いた: ここでプログラムループに入る (SDL_PollEvent()を参照すること)

    SDL_Delay(10000);  // 例として10000ミリ秒間停止する

    // ウィンドウを閉じて破棄する
    SDL_DestroyWindow(window);

    // 終了処理
    SDL_Quit();
    return 0;
}
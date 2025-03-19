#include "raylib.h"
#include "weather.h"

int main(void) {
    // ウィンドウの初期化
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "Weather Effect Sample");
    SetTargetFPS(60);

    // 天候エフェクトの初期化
    WeatherEffect weather;
    InitWeatherEffect(&weather, 100); // 最大100個のパーティクル

    // 天候のタイプと強さを設定
    SetWeatherType(&weather, WEATHER_RAIN);
    SetWeatherPower(&weather, 5); // 強さ 0 ~ 10

    // メインループ
    while (!WindowShouldClose()) {
        // 天候エフェクトの更新
        UpdateWeatherEffect(&weather);

        // 天候のタイプと強さを変更する処理（例）
        if (IsKeyPressed(KEY_ONE)) {
            SetWeatherType(&weather, WEATHER_NONE);
        }
        if (IsKeyPressed(KEY_TWO)) {
            SetWeatherType(&weather, WEATHER_RAIN);
        }
        if (IsKeyPressed(KEY_THREE)) {
            SetWeatherType(&weather, WEATHER_STORM);
        }
        if (IsKeyPressed(KEY_FOUR)) {
            SetWeatherType(&weather, WEATHER_SNOW);
        }
        if (IsKeyPressed(KEY_UP)) {
            int power = weather.power + 1;
            if (power > 10) power = 10;
            SetWeatherPower(&weather, power);
        }
        if (IsKeyPressed(KEY_DOWN)) {
            int power = weather.power - 1;
            if (power < 0) power = 0;
            SetWeatherPower(&weather, power);
        }
        // 描画
        BeginDrawing();
            ClearBackground(BLACK);
            // 天候エフェクトの描画
            DrawWeatherEffect(&weather);

            // ガイドテキストの描画
            DrawText("Press 1: NONE", 10, 10, 20, WHITE);
            DrawText("Press 2: RAIN", 10, 30, 20, WHITE);
            DrawText("Press 3: STORM", 10, 50, 20, WHITE);
            DrawText("Press 4: SNOW", 10, 70, 20, WHITE);
            DrawText("Press UP: Power Up", 10, 90, 20, WHITE);
            DrawText("Press DOWN: Power Down", 10, 110, 20, WHITE);
            DrawText(TextFormat("Power: %d", weather.power), 10, 130, 20, WHITE);
        EndDrawing();
    }

    // 天候エフェクトの解放
    UnloadWeatherEffect(&weather);

    // ウィンドウの終了
    CloseWindow();

    return 0;
}
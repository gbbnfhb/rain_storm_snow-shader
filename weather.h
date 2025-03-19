#ifndef WEATHER_H
#define WEATHER_H

#include "raylib.h"
#ifdef __cplusplus
extern "C" {
#endif

// 天候タイプ
typedef enum {
    WEATHER_NONE = 0,
    WEATHER_RAIN,
    WEATHER_STORM,
    WEATHER_SNOW
} WeatherType;

/*
// パーティクルデータ
typedef struct {
    Vector2 position;
    float opacity;
} Particle;
今はVector3* particles;になってる
*/

// 天候エフェクトの構造体
typedef struct {
    WeatherType type;
    int ox;
    int oy;
    int power;
    int particleCount;
    int maxParticles;
    Vector3* particles;
    Shader shader;
    int shaderResolutionLoc;
    int shaderTimeLoc;
    int shaderParticleCountLoc;
    int shaderParticleDataLoc;
    int shaderWeatherPowerLoc;
    Color dimColor;
    //float *shaderParticleData; // シェーダーに渡すためのデータ配列を追加
} WeatherEffect;

// 天候エフェクトの初期化
void InitWeatherEffect(WeatherEffect *weather, int maxParticles);

// 天候エフェクトの解放
void UnloadWeatherEffect(WeatherEffect *weather);

// 天候エフェクトの更新
void UpdateWeatherEffect(WeatherEffect *weather);

// 天候エフェクトの描画
void DrawWeatherEffect(WeatherEffect *weather);

// 天候エフェクトの強さの設定
void SetWeatherPower(WeatherEffect *weather, int power);

// 天候エフェクトのタイプの変更
void SetWeatherType(WeatherEffect *weather, WeatherType type);

// 天候エフェクトの原点の変更
void SetWeatherOrigin(WeatherEffect *weather, int ox, int oy);

#ifdef __cplusplus
}
#endif

#endif
#include "weather.h"
#include <stdlib.h> // rand, srand
#include <time.h>   // time
#include "raylib-physfs.h"
#define MAROON_H     CLITERAL(Color){ 190, 33, 55, 200 }     // Maroon


float shaderParticleData[100/*weather->maxParticles*/ * 3];

// 天候エフェクトの初期化
void InitWeatherEffect(WeatherEffect *weather, int maxParticles) {
    weather->type = WEATHER_NONE;
    weather->ox = 0;
    weather->oy = 0;
    weather->power = 0;
    weather->particleCount = 0;
    weather->maxParticles = maxParticles;
    weather->particles = (Vector3*)MemAlloc(sizeof(Vector3) * maxParticles);
    weather->shader = LoadShader(0,"rain_storm_snow.fs");
    if(weather->shader.id == 0) {
        TraceLog(LOG_ERROR,"Failed to load shader: rain_storm_snow.frag");
    }
    weather->shaderParticleCountLoc = GetShaderLocation(weather->shader,"u_particle_count");
    weather->shaderParticleDataLoc = GetShaderLocation(weather->shader,"u_particle_data");
    weather->shaderResolutionLoc = GetShaderLocation(weather->shader,"u_resolution");
    weather->shaderWeatherPowerLoc = GetShaderLocation(weather->shader,"u_weather_power");
    weather->dimColor = (Color){ 0, 0, 0, 0 };

    // 乱数シードの初期化
    srand(time(NULL));

    // パーティクルの初期化
    for(int i = 0; i < maxParticles; i++) {
        weather->particles[i].x = (float)GetRandomValue(0,GetScreenWidth());
        weather->particles[i].y = (float)GetRandomValue(0,GetScreenHeight());
        weather->particles[i].z = (float)(160 + GetRandomValue(0,96));
    }
}

// 天候エフェクトの解放
void UnloadWeatherEffect(WeatherEffect *weather) {
    UnloadShader(weather->shader);
    MemFree(weather->particles);
}

// 天候エフェクトの更新
void UpdateWeatherEffect(WeatherEffect *weather) {
    for (int i = 0; i < weather->particleCount; i++) {
        float speedMultiplier = 0.1f + (float)weather->power * 0.2f; // 0.1 ~ 1.0

        switch(weather->type) {
        case WEATHER_RAIN:
            weather->particles[i].x -= 0.1f * speedMultiplier; // ほぼ真下
            weather->particles[i].y += 6.00f * speedMultiplier; // 真下
            weather->particles[i].z -= 1.2;
            break;
        case WEATHER_STORM:
            weather->particles[i].x -= 3.0f * speedMultiplier; // 斜め下
            weather->particles[i].y += 2.00f * speedMultiplier; // 斜め下
            weather->particles[i].z -= 1.2;
            break;
        case WEATHER_SNOW:
            weather->particles[i].x -= 0.2f * speedMultiplier; // 左に移動
            weather->particles[i].y += 1.0f * speedMultiplier; // 下に移動
            weather->particles[i].z -= 1.2;
            break;
        default:
            break;
        }

        if(weather->particles[i].z < 64) {
            weather->particles[i].x = (float)(GetRandomValue(0,GetScreenWidth() + 100) - 100 + weather->ox);
            weather->particles[i].y = (float)(GetRandomValue(0,GetScreenHeight() + 200) - 200 + weather->oy);
            weather->particles[i].z = (float)(160 + GetRandomValue(0,96));
        }
    }
}

// 天候エフェクトの描画
void DrawWeatherEffect(WeatherEffect* weather) {

    // シェーダーにユニフォーム変数を設定
    SetShaderValue(weather->shader,weather->shaderResolutionLoc,(float[2]){ (float)GetScreenWidth(),(float)GetScreenHeight() },SHADER_UNIFORM_VEC2);
    SetShaderValue(weather->shader,weather->shaderParticleCountLoc,(int[1]){ weather->particleCount },SHADER_UNIFORM_INT);
    SetShaderValue(weather->shader,GetShaderLocation(weather->shader,"u_weather_type"),(int[1]){ weather->type },SHADER_UNIFORM_INT);
    SetShaderValueV(weather->shader,weather->shaderParticleDataLoc,(float*)weather->particles,SHADER_UNIFORM_VEC3,weather->particleCount);
    SetShaderValue(weather->shader,weather->shaderWeatherPowerLoc,(int[1]){ weather->power },SHADER_UNIFORM_INT);

    // シェーダーを適用して描画
    BeginShaderMode(weather->shader);
    DrawRectangle(0,0,GetScreenWidth(),GetScreenHeight(),weather->dimColor);
    EndShaderMode();
}

// 天候エフェクトの強さの設定
void SetWeatherPower(WeatherEffect* weather,int power) {
    weather->power = power;
    weather->particleCount = power * 10;
    if(weather->particleCount > weather->maxParticles) {
        weather->particleCount = weather->maxParticles;
    }

    // パーティクルの初期化
    for(int i = 0; i < weather->particleCount; i++) {
        weather->particles[i].x = (float)GetRandomValue(0,GetScreenWidth());
        weather->particles[i].y = (float)GetRandomValue(0,GetScreenHeight());
        weather->particles[i].z = (float)(160 + GetRandomValue(0,96));
    }
}

// 天候エフェクトのタイプの変更
void SetWeatherType(WeatherEffect* weather,WeatherType type) {
    weather->type = type;
}

// 天候エフェクトの原点の変更
void SetWeatherOrigin(WeatherEffect *weather, int ox, int oy) {
    weather->ox = ox;
    weather->oy = oy;
}
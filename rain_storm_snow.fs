#version 330 core

uniform int u_particle_count; // パーティクルの数
uniform vec3 u_particle_data[100]; // パーティクルデータ (x, y, opacity) (必要に応じてサイズを調整)
uniform int u_weather_type;
uniform vec2 u_resolution;
uniform int u_weather_power;

out vec4 fragColor;

// パーティクルの種類 (C言語側の WeatherType と対応)
const int WEATHER_NONE = 0;
const int WEATHER_RAIN = 1;
const int WEATHER_STORM = 2;
const int WEATHER_SNOW = 3;

// パーティクルの色
const vec4 RAIN_COLOR1 = vec4(1.0, 1.0, 1.0, 0.75); // 白、半透明
const vec4 RAIN_COLOR2 = vec4(1.0, 1.0, 1.0, 0.375); // 白、より透明
const vec4 STORM_COLOR1 = vec4(1.0, 1.0, 1.0, 0.75); // 白、半透明
const vec4 STORM_COLOR2 = vec4(1.0, 1.0, 1.0, 0.375); // 白、より透明
const vec4 SNOW_COLOR1 = vec4(1.0, 1.0, 1.0, 0.75); // 白、半透明
const vec4 SNOW_COLOR2 = vec4(1.0, 1.0, 1.0, 0.375); // 白、より透明

void main() {
    vec2 st = gl_FragCoord.xy; // ピクセル座標
    vec4 color = vec4(0.0, 0.0, 0.0, 0.0); // 初期値は透明な黒

    for (int i = 0; i < 100; i++) { // パーティクルをループ処理
        if (i >= u_particle_count) break; // パーティクルの数を超えたら終了

        vec3 particle = u_particle_data[i]; // パーティクルデータ
        vec2 particle_pos = particle.xy; // パーティクルの位置
        float particle_opacity = particle.z; // パーティクルの透明度

        // Y座標を反転
        vec2 inverted_particle_pos = vec2(particle_pos.x, u_resolution.y - particle_pos.y);

        // パーティクルの移動方向を考慮した描画方向を計算
        vec2 particle_direction;
        if (u_weather_type == WEATHER_RAIN) {
            particle_direction = normalize(vec2(-0.2, -4.0)); // ほぼ真下に移動
        } else if (u_weather_type == WEATHER_STORM) {
            particle_direction = normalize(vec2(-3.0, -2.0));
        } else if (u_weather_type == WEATHER_SNOW) {
            particle_direction = normalize(vec2(-0.5, -1.5)); // 左下に移動
        }

        // 現在のピクセルからパーティクルの中心までの距離を計算
        vec2 dist = st - inverted_particle_pos;

        if (u_weather_type == WEATHER_RAIN || u_weather_type == WEATHER_STORM) {
            // 雨と嵐のパーティクル
            float particle_length = (u_weather_type == WEATHER_RAIN) ? (42.0 + float(u_weather_power) * 2.0) : (64.0 + float(u_weather_power) * 2.0); // パーティクルの長さ
            float particle_width = 1.0; // パーティクルの幅

            // パーティクルの向きを考慮した座標変換
            float rotated_x = dot(dist, vec2(particle_direction.y, -particle_direction.x));
            float rotated_y = dot(dist, particle_direction);

            if (abs(rotated_x) < particle_width / 2.0 && rotated_y < 0.0 && rotated_y > -particle_length) {
                float alpha = particle_opacity / 255.0;
                if (rotated_y > -particle_length / 2.0) {
                    color += (u_weather_type == WEATHER_RAIN ? RAIN_COLOR1 : STORM_COLOR1) * alpha;
                } else {
                    color += (u_weather_type == WEATHER_RAIN ? RAIN_COLOR2 : STORM_COLOR2) * alpha;
                }
            }
        } else if (u_weather_type == WEATHER_SNOW) {
            // 雪のパーティクル
            float particle_size = 3.0; // パーティクルの半径
            float dist_to_center = length(dist); // 中心からの距離
            if (dist_to_center < particle_size) {
                float alpha = particle_opacity / 255.0;
                if (dist_to_center < particle_size / 2.0) {
                    color += SNOW_COLOR1 * alpha;
                } else {
                    color += SNOW_COLOR2 * alpha;
                }
            }
        }
    }

    fragColor = color; // 最終的なピクセルの色を設定
}
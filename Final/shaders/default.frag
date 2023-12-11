#version 330 core

out vec4 fragColor;

#define STEPL .01
#define STEPD 2.
#define VOXELS .015

float height(vec2 uv) {
    uv /= 10.0;
    float n = texture(iChannel0, uv).x;
    n += texture(iChannel0, uv * 2.0).x / 2.0;
    n += texture(iChannel0, uv * 4.0).x / 4.0;
    n += texture(iChannel0, uv * 8.0).x / 8.0;
    n *= max(1.0 - abs(length(uv * 20.0)), 0.0);
    return n;
}

vec4 hCol(float h) {
    if (h < 0.05) return vec4(0, 1, 0, 1);
    if (h < 0.1) return vec4(0, 0.9, 0, 1);
    if (h < 0.2) return vec4(0, 0.8, 0, 1);
    if (h < 0.4) return vec4(0, 0.6, 0, 1);
    if (h < 0.6) return vec4(0.8, 0.6, 0.6, 1);
    if (h < 0.8) return vec4(0.8, 0.8, 0.8, 1);
    return vec4(1);
}

void main() {
    vec2 uv = (gl_FragCoord.xy * 2.0 - iResolution.xy) / iResolution.x;
    float st = sin(0.5 * iTime);
    float ct = cos(0.5 * iTime);
    vec3 ro = vec3(ct, 0.2, st);
    vec3 rd = vec3(uv.xy, 1);
    rd = vec3(-rd.x * st - rd.z * ct, rd.y, rd.x * ct - rd.z * st);
    float h;
    int l = int(STEPD / STEPL);
    fragColor = vec4(0, 1, 1, 1);
    for (int i = 0; i < l; i++) {
        ro += rd * STEPL;
        h = height(floor(ro.xz / VOXELS) * VOXELS);
        if (floor(ro.y / VOXELS) * VOXELS <= h / 4.0) {
            fragColor = hCol(h);
            break;
        }
    }
}

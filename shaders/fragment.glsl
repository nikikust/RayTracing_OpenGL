#version 450 core
#define FLT_MAX 3.402823466e+38

#define MAX_REFLECTIONS 8
#define REPEATS_COUNT 16

#define MAX_MATERIALS 128
#define MAX_SPHERES 128

// --- STRUCTURES

// - Objects
struct Sphere {
    vec4 position_and_radius;

    uint material_id;
};

struct Material {
    vec4 albedo;
    vec4 emission_color;
    vec4 blank;

    float roughness;
    float metallic;
    float emissive;
    float emission_power;
};

// - Internal structures
struct HitInfo {
    vec3 hit_origin;
    vec3 normal;
    uint material_id;

    float hit_distance;
};

struct Ray {
    vec3 origin;
    vec3 direction;
};

// --- UNIFORMS

// - Materials

layout(std140, binding = 0) uniform materials_UBO {
    Material[MAX_MATERIALS] materials;
};
uniform int materials_amount;

// - Objects
layout(std140, binding = 1) uniform spheres_UBO {
    Sphere[MAX_SPHERES] spheres;
};
uniform int spheres_amount;

// - Screen
uniform vec2 resolution;
uniform float screen_ratio;
uniform float focus_distance;

// - Scene info
uniform vec3 sun_direction;
uniform vec4 zenith_color;
uniform vec4 horizon_color;

// - Camera
uniform mat3 camera_rotation;
uniform vec3 camera_position;

// - Randomizer
uniform vec2 u_seed1;
uniform vec2 u_seed2;

// --- Additional functions

uvec4 R_STATE;

uint taus_step(uint z, int S1, int S2, int S3, uint M) {
	uint b = (((z << S1) ^ z) >> S2);
	return (((z & M) << S3) ^ b);	
}

uint LCG_step(uint z, uint A, uint C) {
	return (A * z + C);	
}

vec2 hash22(vec2 p) {
	p += u_seed1.x;
	vec3 p3 = fract(vec3(p.xyx) * vec3(.1031, .1030, .0973));
	p3 += dot(p3, p3.yzx+33.33);
	return fract((p3.xx+p3.yz)*p3.zy);
}

float random() {
	R_STATE.x = taus_step(R_STATE.x, 13, 19, 12, uint(4294967294));
	R_STATE.y = taus_step(R_STATE.y, 2, 25, 4, uint(4294967288));
	R_STATE.z = taus_step(R_STATE.z, 3, 11, 17, uint(4294967280));
	R_STATE.w = LCG_step(R_STATE.w, uint(1664525), uint(1013904223));
	return 2.3283064365387e-10 * float((R_STATE.x ^ R_STATE.y ^ R_STATE.z ^ R_STATE.w));
}

vec3 random_on_sphere() {
	vec3 rand = vec3(random(), random(), random());
	float theta = rand.x * 2.0 * 3.14159265;
	float v = rand.y;
	float phi = acos(2.0 * v - 1.0);
	float r = pow(rand.z, 1.0 / 3.0);
	float x = r * sin(phi) * cos(theta);
	float y = r * sin(phi) * sin(theta);
	float z = r * cos(phi);
	return vec3(x, y, z);
}

// --- INTERSECTION FUNCTIONS

HitInfo sphere_intersection(Ray ray, Sphere sphere) {
    vec3 dist = ray.origin - sphere.position_and_radius.xyz;

    float b = dot(ray.direction, dist);
    float c = dot(dist, dist) - sphere.position_and_radius.w * sphere.position_and_radius.w;
    float delta = b * b - c;

    if (delta > 0)
    {
        float t1 = -b + sqrt(delta);
        float t2 = -b - sqrt(delta);

        if (t1 > 0 && t2 > 0)
        {
            vec3 touch_point = ray.origin + ray.direction * (t1 < t2 ? t1 : t2);

            return HitInfo(
                touch_point,
                normalize(touch_point - sphere.position_and_radius.xyz),
                sphere.material_id,
                distance(ray.origin, touch_point));
        }
    }
    return HitInfo(ray.origin, ray.direction, 0, 0.0);
}

// --- Tracer

vec4 trace_ray(Ray ray) {
    vec4 color = vec4(1.0);
    vec4 light = vec4(0.0);

    for (int reflection = 1; reflection <= MAX_REFLECTIONS; ++reflection) {
        HitInfo closest_hit = HitInfo(ray.origin, ray.direction, 0, FLT_MAX);

        for (int i = 0; i < spheres_amount; i++) {
            HitInfo hit = sphere_intersection(ray, spheres[i]);

            if (hit.hit_distance > 0.0 && hit.hit_distance < closest_hit.hit_distance)
                closest_hit = hit;
        }

        if (closest_hit.hit_distance == FLT_MAX) {
            light += mix(horizon_color, zenith_color, sqrt(abs(ray.direction.z))) * color;

            break;
        }
        else {
            Material material = materials[closest_hit.material_id];
            if (material.emissive == 1.0) {
                light += material.emission_color * material.emission_power * color;
            }

            color *= material.albedo;

            vec3 reflected = reflect(ray.direction, closest_hit.normal);

            vec3 random_direction = random_on_sphere();
	        vec3 diffuse = random_direction * sign(dot(random_direction, closest_hit.normal));

            ray.direction  = mix(reflected, diffuse, material.roughness);
            ray.origin     = closest_hit.hit_origin + ray.direction * 0.001;
        }
    }
        
    return light;
}

// --- Main

out vec4 fragColor;

void main() {
    vec2 pos = vec2(2.0 * gl_FragCoord.xy / resolution.xy - 1.0);

    vec2 uvRes = hash22(pos + 1.0) * resolution + resolution;
	R_STATE.x = uint(u_seed1.x + uvRes.x);
	R_STATE.y = uint(u_seed1.y + uvRes.x);
	R_STATE.z = uint(u_seed2.x + uvRes.y);
	R_STATE.w = uint(u_seed2.y + uvRes.y);

    vec3 ray_direction = normalize(
        vec3(pos.x * screen_ratio,
             focus_distance,
             pos.y
        )
    );

    Ray ray = Ray(camera_position, ray_direction * camera_rotation);

    vec4 resulting_color = vec4(0);

    for (int repeats = 1; repeats <= REPEATS_COUNT; ++repeats) {
        resulting_color += trace_ray(ray);
    }

    fragColor = resulting_color / REPEATS_COUNT;
}

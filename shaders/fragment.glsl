#version 450 core
#define FLT_MAX 3.402823466e+38

#define MAX_SPHERES 128
#define MAX_REFLECTIONS 16

// --- STRUCTURES

// - Objects
struct Sphere {
    vec4 position_and_radius;
    vec4 color;
};

// - Internal structures
struct HitInfo {
    vec3 hit_origin;
    vec3 normal;
    vec4 color;

    float hit_distance;
};

struct Ray {
    vec3 origin;
    vec3 direction;
};

// --- UNIFORMS

// - Objects
layout(std140, binding = 0) uniform spheres_UBO {
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

// --- INTERSECTION FUNCTIONS

HitInfo sphere_intersection(in Ray ray, in Sphere sphere) {
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

            return HitInfo(touch_point, normalize(touch_point - sphere.position_and_radius.xyz), sphere.color, distance(ray.origin, touch_point));
        }
    }
    return HitInfo(ray.origin, ray.direction, vec4(0.0), 0.0);
}


// --- Tracer

vec4 trace_ray(in Ray ray){
    vec4 color = vec4(1.0);

    for (int reflection = 1; reflection <= MAX_REFLECTIONS; ++reflection)
    {
        HitInfo closest_hit = HitInfo(ray.origin, ray.direction, vec4(0.0), FLT_MAX);

        for (int i = 0; i < spheres_amount; i++){
            HitInfo hit = sphere_intersection(ray, spheres[i]);

            if (hit.hit_distance > 0.0 && hit.hit_distance < closest_hit.hit_distance)
                closest_hit = hit;
        }

        if (closest_hit.hit_distance == FLT_MAX) {
            color *= mix(horizon_color, zenith_color, sqrt(abs(ray.direction.z)));
            break;
        } else {
            color *= closest_hit.color;

            ray.direction = reflect(ray.direction, closest_hit.normal);

            ray.origin = closest_hit.hit_origin + ray.direction * 0.001;
        }

        if (reflection == MAX_REFLECTIONS) {
            color = vec4(0.0, 0.0, 0.0, 1.0);
        }
    }

    return color;
}

// --- Main

out vec4 fragColor;

void main() {
    vec2 pos = vec2(2.0 * gl_FragCoord.x / resolution.x - 1.0,
                    2.0 * gl_FragCoord.y / resolution.y - 1.0);

    vec3 ray_direction = normalize(
        vec3(pos.x * screen_ratio,
             focus_distance,
             pos.y
        )
    );

    Ray ray = Ray(camera_position, ray_direction * camera_rotation);

    fragColor = trace_ray(ray);
}

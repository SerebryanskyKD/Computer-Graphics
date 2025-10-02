#define KEY_LEFT 37
#define KEY_RIGHT 39

float rand(float n) {
    return fract(sin(n) * 43758.5453);
}

float sdCircle(vec2 p, float r)
{
    return length(p) - r;
}

float sdBox(vec2 p, vec2 size)
{
    vec2 d = abs(p) - size;
    return max(d.x, d.y);
}

float sdCar(vec2 pos, vec2 carSize)
{
    float body = sdBox(pos - vec2(0.0, 0.0), carSize * vec2(0.8, 0.5));
    float hood = sdBox(pos - vec2(0.0, 0.15), carSize * vec2(0.5, 0.3));
    return min(body, hood);
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = fragCoord / iResolution.xy;
    vec2 centered_uv = uv * 2.0 - 1.0;
    
    vec3 color = texture(iChannel1, uv).xyz;
    
    float carX = 0.0;
    
    if (texelFetch(iChannel0, ivec2(KEY_RIGHT, 0), 0).x > 0.5) {
        carX += 0.5;
    }
    if (texelFetch(iChannel0, ivec2(KEY_LEFT, 0), 0).x > 0.5) {
        carX -= 0.5;
    }
    
    vec2 carPos = centered_uv - vec2(carX, -0.6);
    
    float obstacleSpeed = 1.5;
    float obstacleY = 1.0 - mod(iTime * obstacleSpeed, 2.0);
    
    float obstacleX = mix(-0.9, 0.9, rand(floor(iTime * obstacleSpeed / 2.0)));
    
    vec2 obstaclePos = centered_uv - vec2(obstacleX, obstacleY);
    
    float obstacle = sdCircle(obstaclePos, 0.1);
    if(obstacle < 0.0) {
        color = vec3(1.0, 0.0, 0.0);
    }
    
    float car = sdCar(carPos, vec2(0.1, 0.3));
    
    if(car < 0.0) {
        color = vec3(0.9, 0.1, 0.1);
    }

    fragColor = vec4(color, 1.0);
}

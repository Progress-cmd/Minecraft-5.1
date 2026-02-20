#version 330 core // indique la version d'OpenGL


out vec4 FragColor; // créé une couleur de type vec4


in vec2 texCoord; // défini une entrée

in vec3 crntPos; // Position reçue du vertex shader


uniform sampler2D tex0; // indique l'unité de texture à utiliser

uniform bool verticeMode; // activation ou non du mode vertice

uniform vec3 camPos; // On a besoin de la position de la caméra

uniform int maxDistance; // Distance max de simulation


void main()
{
    vec4 texColor = texture(tex0, texCoord);
    if(texColor.a < 0.1) discard; // Transparence basique

    // 1. Calcul de la distance réelle (circulaire)
    vec2 diff = abs(crntPos.xz - camPos.xz);
    float dist = max(diff.x, diff.y); // Brouillard carré

    // FIX : On définit la limite stricte
    float limit = float(maxDistance * 16);

    // 2. Paramètres du brouillard
    // Le brouillard commence à 40% et finit à 95% de la distance max
    // On finit à 95% pour être SÛR que c'est opaque avant la coupure brutale
    float fogNear = limit * 0.4; 
    float fogFar = limit * 0.95;

    vec4 skyColor = vec4(0.07, 0.13, 0.17, 1.0); // La même couleur que ton glClearColor

    // 3. Calcul du facteur de mélange (entre 0.0 et 1.0)
    float fogFactor = clamp((dist - fogNear) / (fogFar - fogNear), 0.0, 1.0);
    fogFactor = smoothstep(0.0, 1.0, fogFactor);

    // 4. Mélange final
    FragColor = vec4(mix(texColor.rgb, skyColor.rgb, fogFactor), texColor.a);
}
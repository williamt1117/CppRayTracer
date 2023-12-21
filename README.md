# CppRayTracer

This project utilizes the [OpenGL Mathematics library](https://github.com/g-truc/glm) to create a basic sphere ray tracer in C++ that reads an input file and renders a scene utilizing backwards ray tracing and the Phong illumination model to output a ppm (portable pixmap format) image.

![testRainbow](https://github.com/williamt1117/CppRayTracer/assets/92940760/4bc66712-bbee-4297-a647-a8b7ace9e079)

[Figure 1] All-Inclusive Rainbow Deep Reflection Test

## Compiling and Running

Compiling this project requires the GLM library in an "include" folder, "main.cpp", and "Makefile" at the same height. Compile with the terminal command `make` to output the file 'RayTracer.exe'. The project is run through the terminal with `./RayTracer.exe <inputFile.txt>`. The input file must be specially formatted as follows:
```
NEAR <nearPlaneDistance>
LEFT <leftImagePlanePosition>
RIGHT <rightImagePlanePosition>
BOTTOM <bottomImagePlanePosition>
TOP <topImagePlanePosition>
RES <resolutionX> <resolutionY>
SPHERE <namei> <positionX> <positionY> <positionZ> <scaleX> <scaleY> <scaleZ> <colorR> <colorG> <colorB> <ambient> <diffuse> <specular> <reflectivity> <specularExponent>
...
...
SPHERE <namej> <positionX> <positionY> <positionZ> <scaleX> <scaleY> <scaleZ> <colorR> <colorG> <colorB> <ambient> <diffuse> <specular> <reflectivity> <specularExponent>
LIGHT <namei> <positionX> <positionY> <positionZ> <intensityR> <intensityG> <intensityB>
...
...
LIGHT <namej> <positionX> <positionY> <positionZ> <intensityR> <intensityG> <intensityB>
BACK <backgroundColorR> <backgroundColorG> <backgroundColorB>
AMBIENT <ambientIntensityR> <ambientIntensityG> <ambientIntensityB>
OUTPUT <outputFileName.ppm>
```

It is assumed that all values that are not strings, besides the specular exponent and resolution, are floats as opposed to integers. Additionally, the eye/camera is situated at the origin and looks down the negative z-axis.

## Testing

Included in the "Assignment3-Tests-and-Keys" subdirectory are 12 test cases prefixed "test" paired with the intended output prefixed "key". Alongside this, the C script titled "makeInputFile" can be used with the command `./makeInputFile.exe <sphereCount> <lightCount>` to generate a random test file titled 'testLots.txt' with a specified amount of spheres and lights keeping a fixed image plane and resolution and outputting 'testLots.ppm'.

## Results

The program successfully implements the following:
- Ambient lighting
- Diffuse lighting
- Specular lighting
- Recursive reflection
- Internal collisions
- Near-plane clipping
- Shadows
- Intersecting geometry
- Supports transformed spheres (ellipsoids)

The program has one undiagnosed error that appears when mass amounts of objects are rendered that causes black noise to appear on occasional distant objects, as shown below.

![visualArtifact](https://github.com/williamt1117/CppRayTracer/assets/92940760/ef536bb2-fd42-49d5-8e27-cb8c132e7cb8)

[Figure 2] Black Noise Visual Artifact


Noteworthy successful tests are shown below:

![testRainbow](https://github.com/williamt1117/CppRayTracer/assets/92940760/4bc66712-bbee-4297-a647-a8b7ace9e079)

[Figure 1] All-Inclusive Rainbow Deep Reflection Test

![test1](https://github.com/williamt1117/CppRayTracer/assets/92940760/81141e0b-73ef-4326-866a-50d2b7b26402)

[Figure 3] Mass Test 1


![test2](https://github.com/williamt1117/CppRayTracer/assets/92940760/c65f3e70-ce79-42c3-ad0e-a89b95a66162)

[Figure 4] Mass Test 2


![testSample](https://github.com/williamt1117/CppRayTracer/assets/92940760/1051db26-279a-46ff-9964-f3aa303a05a5)

[Figure 5] All-Inclusive Sample Test


![testAmbient](https://github.com/williamt1117/CppRayTracer/assets/92940760/087e5a6a-7c87-4246-85cd-37790f5e2afc)

[Figure 6] Ambient Lighting Test


![testDiffuse](https://github.com/williamt1117/CppRayTracer/assets/92940760/c0b9adfb-549e-4a40-b6e9-755799219a0b)

[Figure 7] Diffuse Lighting Test


![testSpecular](https://github.com/williamt1117/CppRayTracer/assets/92940760/75616690-051e-4372-a1d5-2be87e0228e7)

[Figure 8] Specular Lighting Test


![testIllum](https://github.com/williamt1117/CppRayTracer/assets/92940760/6a212fc8-4ea1-4cbd-9b9d-ec820db0d128)

[Figure 9] Illuminant Lighting Test


![testReflection](https://github.com/williamt1117/CppRayTracer/assets/92940760/2d2f7c35-c16d-4de9-a3f1-7efb929dd207)

[Figure 10] Reflection Test


![testShadow](https://github.com/williamt1117/CppRayTracer/assets/92940760/2d360bfa-2c9a-49dd-90f0-920174e1d809)

[Figure 11] Shadows Test


### Future Work

Possible improvements to this project could involve:
- Create visual window to allow real-time rendering
- Add refraction
- Support other shapes/geometries
- Repair black noise that appears in mass tests

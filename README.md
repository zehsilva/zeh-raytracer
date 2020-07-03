# zeh-raytracer

RayTracer project developed for Computer Graphics course at Federal University of Espirito Santo.

---

O que foi implementado:
- supersampling;
- depth of field;
- reflexão;
- interseção: plano, triangulo, esfera;
- iluminação e sombra;


O arquivo nff segue o padrão normal, mas tem alguns tokens novos:

** Shaders
fl r g b - para adicionar o FlatShader
flam r g b - lambertian shader diffuse simples
f r g b ..... - PhongShader



**Camera
1) Camera para efeito de "depth of field" -> token vd...
exemplo:
vd
from 0 10 10
at 0 0 0
up 1 2 0
angle 60
lensRadius 0.05
focalDistance 13.4
resolution 400 200

2)Ortogonal (vo)
vo
from 0 10 10
at -0 1 4
up 1 0 0
angle 90
hither 0
resolution 300 400

3) Perspective (v)

**Arquivos externos 
1) OFF - tag includeO seguido, na nova linha, pelo nome do arquivo off 
includeO 
humanoid.off

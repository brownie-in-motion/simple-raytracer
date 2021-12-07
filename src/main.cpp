# include <iostream>
# include <string>
# include <vector>

# include "r3_vec.h"
# include "ray.h"
# include "scene.h"
# include "sphere.h"
# include "sphere-texture.h"

void write_image(
    std::vector<std::vector<r3_vec>> pixels,
    std::string filename
) {
    FILE *f = fopen(filename.c_str(), "w");

    fprintf(f, "P3\n");
    fprintf(f, "%ld %ld", pixels.size(), pixels[0].size());
    fprintf(f, "\n255\n");

    for (int i = pixels.size() - 1; i >= 0; i--) {
        for (int j = 0; j < pixels[i].size(); j++) {
            fprintf(f, "%d %d %d ",
                (int) pixels[i][j].x(),
                (int) pixels[i][j].y(),
                (int) pixels[i][j].z()
            );
        }
    }
}

int main() {
    scene s = scene();

    texture* matte = new matte_texture(r3_vec(0, 0, 0));
    texture* shiny = new mirror_texture(0.85);

    sphere ground = sphere(r3_vec(0, -1e8 - 200, 0), 1e8, matte);
    sphere o = sphere(r3_vec(-350, 100, 500), 300, matte);
    sphere p = sphere(r3_vec(350, 100, 500), 300, shiny);

    s.add_object(&ground);
    s.add_object(&o);
    s.add_object(&p);

    srand(time(NULL));

    std::vector<r3_vec> small_balls = {
        r3_vec(-65, -150, 500),
        r3_vec(300, -150, 10),
        r3_vec(180, -150, 0),
        r3_vec(-850, -150, 1000),
    };

    for (int i = 0; i < small_balls.size(); i++) {
        sphere* p = new sphere(
            small_balls[i],
            50,
            shiny // shiny
        );
        s.add_object(p);
    }

    r3_vec light = r3_vec(5000, 10000, -5000);
    s.add_light(&light);

    sphere fake_lamp = sphere(r3_vec(7000, 12000, -7000), 1000, matte);
    s.add_object(&fake_lamp);

    write_image(s.render(
            r3_vec(0, 0, -800), // camera
            1000, // width and height
            1000,
            20 // samples
    ), "out.ppm");
}

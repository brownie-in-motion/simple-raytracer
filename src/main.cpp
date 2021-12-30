# include <iostream>
# include <string>
# include <vector>

# include "r3_vec.h"
# include "ray.h"
# include "scene.h"
# include "sphere.h"
# include "textures.h"

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

    texture* matte_red = new matte_texture(new flat(r3_vec(255, 20, 20)));
    texture* matte_blue = new matte_texture(new flat(r3_vec(20, 20, 255)));
    texture* matte_white = new matte_texture(new flat(r3_vec(220, 220, 220)));
    texture* matte_checkered = new matte_texture(new checkers(
        r3_vec(255, 255, 255),
        r3_vec(200, 200, 200),
        200
    ));
    texture* shiny = new mirror_texture(0.85);
    texture* glass = new glass_texture(1.5);

    sphere ground = sphere(r3_vec(0, -1e8 - 200, 0), 1e8, matte_checkered);
    sphere o = sphere(r3_vec(-400, 0, 500), 200, matte_red);
    sphere g = sphere(r3_vec(0, 0, 500), 200, glass);
    sphere m = sphere(r3_vec(400, 0, 500), 200, shiny);

    s.add_object(&ground);
    s.add_object(&o);
    // s.add_object(&g);
    s.add_object(&m);

    srand(time(NULL));

    std::vector<r3_vec> small_balls = {
        r3_vec(-65, -150, 2000),
        r3_vec(-200, -150, 2000),
        r3_vec(300, -150, 10),
        r3_vec(180, -150, 0),
        r3_vec(-850, -150, 1000),
    };

    std::vector<texture*> textures ={
        shiny,
        matte_white,
        matte_blue,
        shiny,
        shiny
    };

    for (int i = 0; i < small_balls.size(); i++) {
        sphere* p = new sphere(
            small_balls[i],
            50,
            textures[i]
        );
        s.add_object(p);
    }

    light l = light(r3_vec(5000, 10000, -5000), r3_vec(255, 255, 255));
    s.add_light(&l);

    // light dimmer = light(r3_vec(-2000, 5000, -5000), r3_vec(40, 40, 40));
    // s.add_light(&dimmer);

    write_image(s.render(
            r3_vec(0, 0, -800), // camera
            1000, // width and height
            1000,
            1 // samples
    ), "out.ppm");
}

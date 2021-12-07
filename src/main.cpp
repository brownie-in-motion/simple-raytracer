# include <iostream>
# include <string>
# include <vector>

# include "r3_vec.h"
# include "ray.h"
# include "scene.h"
# include "sphere.h"

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

    sphere ground = sphere(r3_vec(0, -1e8 - 200, 0), 1e8, false);
    sphere o = sphere(r3_vec(-350, 100, 500), 300, false);
    sphere p = sphere(r3_vec(350, 100, 500), 300, true);

    s.add_object(&ground);
    s.add_object(&o);
    s.add_object(&p);

    srand(time(NULL));

    for (int i = 0; i < 10; i++) {
        double distance = rand() % 1000;
        double horizontal = rand() % 2000;
        sphere* p = new sphere(
            r3_vec(horizontal - 1000, -150, distance),
            50,
            false
        );
        s.add_object(p);
    }

    r3_vec light = r3_vec(5000, 10000, -5000);
    s.add_light(&light);

    write_image(s.render(
            r3_vec(0, 0, -800), // camera
            1000, // width and height
            1000,
            10 // samples
    ), "out.ppm");
}

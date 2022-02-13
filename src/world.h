//
// Created by Vimii on 2022/02/13.
//

#ifndef SPECTRAL_RAYTRACING_WORLD_H
#define SPECTRAL_RAYTRACING_WORLD_H

hittable_list cornell_box(camera& cam, double aspect) {
    hittable_list world;

    auto red_data = "400:0.04, 404:0.046, 408:0.048, 412:0.053, 416:0.049, 420:0.05, 424:0.053, 428:0.055, 432:0.057, 436:0.056, 440:0.059, 444:0.057, 448:0.061, 452:0.061, 456:0.06, 460:0.062, 464:0.062, 468:0.062, 472:0.061, 476:0.062, 480:0.06, 484:0.059, 488:0.057, 492:0.058, 496:0.058, 500:0.058, 504:0.056, 508:0.055, 512:0.056, 516:0.059, 520:0.057, 524:0.055, 528:0.059, 532:0.059, 536:0.058, 540:0.059, 544:0.061, 548:0.061, 552:0.063, 556:0.063, 560:0.067, 564:0.068, 568:0.072, 572:0.08, 576:0.09, 580:0.099, 584:0.124, 588:0.154, 592:0.192, 596:0.255, 600:0.287, 604:0.349, 608:0.402, 612:0.443, 616:0.487, 620:0.513, 624:0.558, 628:0.584, 632:0.62, 636:0.606, 640:0.609, 644:0.651, 648:0.612, 652:0.61, 656:0.65, 660:0.638, 664:0.627, 668:0.62, 672:0.63, 676:0.628, 680:0.642, 684:0.639, 688:0.657, 692:0.639, 696:0.635, 700:0.642";
    auto white_data = "400:0.343, 404:0.445, 408:0.551, 412:0.624, 416:0.665, 420:0.687, 424:0.708, 428:0.723, 432:0.715, 436:0.71, 440:0.745, 444:0.758, 448:0.739, 452:0.767, 456:0.777, 460:0.765, 464:0.751, 468:0.745, 472:0.748, 476:0.729, 480:0.745, 484:0.757, 488:0.753, 492:0.75, 496:0.746, 500:0.747, 504:0.735, 508:0.732, 512:0.739, 516:0.734, 520:0.725, 524:0.721, 528:0.733, 532:0.725, 536:0.732, 540:0.743, 544:0.744, 548:0.748, 552:0.728, 556:0.716, 560:0.733, 564:0.726, 568:0.713, 572:0.74, 576:0.754, 580:0.764, 584:0.752, 588:0.736, 592:0.734, 596:0.741, 600:0.74, 604:0.732, 608:0.745, 612:0.755, 616:0.751, 620:0.744, 624:0.731, 628:0.733, 632:0.744, 636:0.731, 640:0.712, 644:0.708, 648:0.729, 652:0.73, 656:0.727, 660:0.707, 664:0.703, 668:0.729, 672:0.75, 676:0.76, 680:0.751, 684:0.739, 688:0.724, 692:0.73, 696:0.74, 700:0.737";
    auto green_data = "400:0.092, 404:0.096, 408:0.098, 412:0.097, 416:0.098, 420:0.095, 424:0.095, 428:0.097, 432:0.095, 436:0.094, 440:0.097, 444:0.098, 448:0.096, 452:0.101, 456:0.103, 460:0.104, 464:0.107, 468:0.109, 472:0.112, 476:0.115, 480:0.125, 484:0.14, 488:0.16, 492:0.187, 496:0.229, 500:0.285, 504:0.343, 508:0.39, 512:0.435, 516:0.464, 520:0.472, 524:0.476, 528:0.481, 532:0.462, 536:0.447, 540:0.441, 544:0.426, 548:0.406, 552:0.373, 556:0.347, 560:0.337, 564:0.314, 568:0.285, 572:0.277, 576:0.266, 580:0.25, 584:0.23, 588:0.207, 592:0.186, 596:0.171, 600:0.16, 604:0.148, 608:0.141, 612:0.136, 616:0.13, 620:0.126, 624:0.123, 628:0.121, 632:0.122, 636:0.119, 640:0.114, 644:0.115, 648:0.117, 652:0.117, 656:0.118, 660:0.12, 664:0.122, 668:0.128, 672:0.132, 676:0.139, 680:0.144, 684:0.146, 688:0.15, 692:0.152, 696:0.157, 700:0.159";
    auto red   = make_shared<lambertian>(make_shared<solid_color>(stospec(red_data)));
    auto white = make_shared<lambertian>(make_shared<solid_color>(spectrum(1.0)));
    auto green = make_shared<lambertian>(make_shared<solid_color>(stospec(green_data)));
    auto light = make_shared<diffuse_light>(make_shared<solid_color>(D65*0.01));
    auto glass = make_shared<dielectric>(1.5,0);

    for (int i = 0; i < SAMPLE_NUM; ++i) {
        std::cout << std::setprecision(10) << stospec(red_data)[i] << std::endl;
    }

    std::cout << std::setprecision(10) << xyz_to_srgb(spectrum_to_xyz(D65* stospec(red_data))) << std::endl;

    shared_ptr<material> aluminum =
            make_shared<metal>(stospec(white_data), 0.0);

    world.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    world.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    world.add(make_shared<flip_face>(make_shared<xz_rect>(200, 350, 227, 332, 554, light)));
    world.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    world.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    world.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    shared_ptr<hittable> box1 =
            make_shared<box>(point3(0,0,0), point3(165,330,165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265,0,295));
    world.add(box1);

    world.add(make_shared<sphere>(point3(190, 90, 190), 90, glass));


//        world.add(make_shared<box>(point3(0,450,0), point3(260,550,555), white));
//        world.add(make_shared<box>(point3(270,450,0), point3(555,550,555), white));


//    shared_ptr<hittable> box1 = make_shared<box>(point3(0,0,0), point3(200,100,100), glass);
//    box1 = make_shared<rotate_x>(box1, 40);
//    box1 = make_shared<rotate_y>(box1, 90);
//    box1 = make_shared<translate>(box1, vec3(200,370,395));
//    world.add(box1);

    //全体
    point3 lookfrom(278, 278, -800);
    point3 lookat(278, 278, 0);
    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.0;
    auto vfov = 40.0;
    auto t0 = 0.0;
    auto t1 = 1.0;
//
//    point3 lookfrom(278, 278, -800);
//    point3 lookat(278, 178, 0);
//    vec3 vup(0, 1, 0);
//    auto dist_to_focus = 10.0;
//    auto aperture = 0.0;
//    auto vfov = 20.0;
//    auto t0 = 0.0;
//    auto t1 = 1.0;

    cam = camera(lookfrom, lookat, vup, vfov, aspect, aperture, dist_to_focus, t0, t1);

    return world;
}

hittable_list cornell_box(camera& cam, double aspect, double t_ratio) {
    hittable_list world;

    auto red_data = "400:0.04, 404:0.046, 408:0.048, 412:0.053, 416:0.049, 420:0.05, 424:0.053, 428:0.055, 432:0.057, 436:0.056, 440:0.059, 444:0.057, 448:0.061, 452:0.061, 456:0.06, 460:0.062, 464:0.062, 468:0.062, 472:0.061, 476:0.062, 480:0.06, 484:0.059, 488:0.057, 492:0.058, 496:0.058, 500:0.058, 504:0.056, 508:0.055, 512:0.056, 516:0.059, 520:0.057, 524:0.055, 528:0.059, 532:0.059, 536:0.058, 540:0.059, 544:0.061, 548:0.061, 552:0.063, 556:0.063, 560:0.067, 564:0.068, 568:0.072, 572:0.08, 576:0.09, 580:0.099, 584:0.124, 588:0.154, 592:0.192, 596:0.255, 600:0.287, 604:0.349, 608:0.402, 612:0.443, 616:0.487, 620:0.513, 624:0.558, 628:0.584, 632:0.62, 636:0.606, 640:0.609, 644:0.651, 648:0.612, 652:0.61, 656:0.65, 660:0.638, 664:0.627, 668:0.62, 672:0.63, 676:0.628, 680:0.642, 684:0.639, 688:0.657, 692:0.639, 696:0.635, 700:0.642";
    auto white_data = "400:0.343, 404:0.445, 408:0.551, 412:0.624, 416:0.665, 420:0.687, 424:0.708, 428:0.723, 432:0.715, 436:0.71, 440:0.745, 444:0.758, 448:0.739, 452:0.767, 456:0.777, 460:0.765, 464:0.751, 468:0.745, 472:0.748, 476:0.729, 480:0.745, 484:0.757, 488:0.753, 492:0.75, 496:0.746, 500:0.747, 504:0.735, 508:0.732, 512:0.739, 516:0.734, 520:0.725, 524:0.721, 528:0.733, 532:0.725, 536:0.732, 540:0.743, 544:0.744, 548:0.748, 552:0.728, 556:0.716, 560:0.733, 564:0.726, 568:0.713, 572:0.74, 576:0.754, 580:0.764, 584:0.752, 588:0.736, 592:0.734, 596:0.741, 600:0.74, 604:0.732, 608:0.745, 612:0.755, 616:0.751, 620:0.744, 624:0.731, 628:0.733, 632:0.744, 636:0.731, 640:0.712, 644:0.708, 648:0.729, 652:0.73, 656:0.727, 660:0.707, 664:0.703, 668:0.729, 672:0.75, 676:0.76, 680:0.751, 684:0.739, 688:0.724, 692:0.73, 696:0.74, 700:0.737";
    auto green_data = "400:0.092, 404:0.096, 408:0.098, 412:0.097, 416:0.098, 420:0.095, 424:0.095, 428:0.097, 432:0.095, 436:0.094, 440:0.097, 444:0.098, 448:0.096, 452:0.101, 456:0.103, 460:0.104, 464:0.107, 468:0.109, 472:0.112, 476:0.115, 480:0.125, 484:0.14, 488:0.16, 492:0.187, 496:0.229, 500:0.285, 504:0.343, 508:0.39, 512:0.435, 516:0.464, 520:0.472, 524:0.476, 528:0.481, 532:0.462, 536:0.447, 540:0.441, 544:0.426, 548:0.406, 552:0.373, 556:0.347, 560:0.337, 564:0.314, 568:0.285, 572:0.277, 576:0.266, 580:0.25, 584:0.23, 588:0.207, 592:0.186, 596:0.171, 600:0.16, 604:0.148, 608:0.141, 612:0.136, 616:0.13, 620:0.126, 624:0.123, 628:0.121, 632:0.122, 636:0.119, 640:0.114, 644:0.115, 648:0.117, 652:0.117, 656:0.118, 660:0.12, 664:0.122, 668:0.128, 672:0.132, 676:0.139, 680:0.144, 684:0.146, 688:0.15, 692:0.152, 696:0.157, 700:0.159";
    auto red   = make_shared<lambertian>(make_shared<solid_color>(stospec(red_data)));
    auto white = make_shared<lambertian>(make_shared<solid_color>(stospec(white_data)));
    auto green = make_shared<lambertian>(make_shared<solid_color>(stospec(green_data)));
    auto gray = make_shared<lambertian>(make_shared<solid_color>(spectrum(0.3f)));
    auto black = make_shared<lambertian>(make_shared<solid_color>(spectrum(0.f)));
    auto light = make_shared<diffuse_light>(make_shared<solid_color>(D65*100));
    shared_ptr<material> aluminum =
            make_shared<metal>(stospec(red_data), 0.0);

    world.add(make_shared<yz_rect>(0, 555, 0, 555, 555, white));
    world.add(make_shared<yz_rect>(0, 555, 0, 555, 0, white));
    world.add(make_shared<flip_face>(make_shared<xz_rect>(200, 350, 227, 332, 554, light)));
    world.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    world.add(make_shared<xz_rect>(0, 555, 0, 555, 0, gray));
    world.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    world.add(make_shared<box>(point3(0,450,0), point3(260,550,555), white));
    world.add(make_shared<box>(point3(270,450,0), point3(555,550,555), white));

    auto glass = make_shared<dielectric>(1.5, 1);
    shared_ptr<hittable> box1 = make_shared<box>(point3(0,0,0), point3(200,100,100), glass);
    box1 = make_shared<rotate_x>(box1, 20 * (1 - t_ratio) + 70 * t_ratio);
    box1 = make_shared<rotate_y>(box1, 90);
    box1 = make_shared<translate>(box1, vec3(200,170,395));
    world.add(box1);

//    auto dia = make_shared<dielectric>(2.416,0.7);
//    shared_ptr<hittable> diamond = make_shared<ObjModel>("../resource/obj/diamond.obj", dia);
//    diamond = make_shared<rotate_y>(diamond, 40 * t_ratio);
//    diamond =  make_shared<translate>(diamond, vec3(278,130,200));
//    world.add(diamond);

    //三角形
//    std::vector<Vector3f> verticies;
//    verticies.push_back(Vector3f(200,0,200));
//    verticies.push_back(Vector3f(0,-100,400));
//    verticies.push_back(Vector3f(100,0,300));
//
//    world.add(make_shared<translate>(make_shared<triangle>(verticies, red), vec3(200,170,0)));



//    //diamond
//    point3 lookfrom(310, 500 * t_ratio + 200 * (1.0-t_ratio), 400 * t_ratio + 500 * (1.0-t_ratio));
//    point3 lookat(278, 50, 200);
//    vec3 vup(0, 1, 0);
//    auto dist_to_focus = 10.0;
//    auto aperture = 0.0;
//    auto vfov = 60.0;
//    auto t0 = 0.0;
//    auto t1 = 1.0;

    point3 lookfrom(278, 278, -800);
    point3 lookat(278, 178, 0);
    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.0;
    auto vfov = 20.0;
    auto t0 = 0.0;
    auto t1 = 1.0;

//    //全体
//    point3 lookfrom(278, 278, -800);
//    point3 lookat(278, 278, 0);
//    vec3 vup(0, 1, 0);
//    auto dist_to_focus = 10.0;
//    auto aperture = 0.0;
//    auto vfov = 40.0;
//    auto t0 = 0.0;
//    auto t1 = 1.0;

    cam = camera(lookfrom, lookat, vup, vfov, aspect, aperture, dist_to_focus, t0, t1);

    return world;
}

#endif //SPECTRAL_RAYTRACING_WORLD_H

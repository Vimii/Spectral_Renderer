//
// Created by Vimii on 2021/12/07.
//
#include <iostream>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../stb-master/stb_image_write.h"
#include <memory>
#include <new>
#include "time.h"
#include "util.h"
#include "filename.h"
#include "spectrum.h"
#include "d65.h"
#include "hittable.h"
#include "material.h"
#include "camera.h"
#include "hittable_list.h"
#include "aarect.h"
#include "sphere.h"
#include "box.h"
#include <string.h>
#include <omp.h>

int main(){
    spectrum RED = spectrum_type(_RED);
    std::string value = "400:0.04, 404:0.046, 408:0.048, 412:0.053, 416:0.049, 420:0.05, 424:0.053, 428:0.055, 432:0.057, 436:0.056, 440:0.059, 444:0.057, 448:0.061, 452:0.061, 456:0.06, 460:0.062, 464:0.062, 468:0.062, 472:0.061, 476:0.062, 480:0.06, 484:0.059, 488:0.057, 492:0.058, 496:0.058, 500:0.058, 504:0.056, 508:0.055, 512:0.056, 516:0.059, 520:0.057, 524:0.055, 528:0.059, 532:0.059, 536:0.058, 540:0.059, 544:0.061, 548:0.061, 552:0.063, 556:0.063, 560:0.067, 564:0.068, 568:0.072, 572:0.08, 576:0.09, 580:0.099, 584:0.124, 588:0.154, 592:0.192, 596:0.255, 600:0.287, 604:0.349, 608:0.402, 612:0.443, 616:0.487, 620:0.513, 624:0.558, 628:0.584, 632:0.62, 636:0.606, 640:0.609, 644:0.651, 648:0.612, 652:0.61, 656:0.65, 660:0.638, 664:0.627, 668:0.62, 672:0.63, 676:0.628, 680:0.642, 684:0.639, 688:0.657, 692:0.639, 696:0.635, 700:0.642";
    auto elems = stospec(value);
    std::cout << elems[45] << std::endl;
    return 0;
}

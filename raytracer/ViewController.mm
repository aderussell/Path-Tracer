//
//  ViewController.m
//  raytracer
//
//  Created by Adrian Russell on 29/06/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#import "ViewController.h"
#include "RayTracer.hpp"

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];

    // Do any additional setup after loading the view.
    
    RayTracer rayTracer = RayTracer(500, 500);
    ImageBuffer *buffer = rayTracer.render();
    
    NSBitmapImageRep *bitmapImage = [[NSBitmapImageRep alloc] initWithBitmapDataPlanes:NULL
                                                                            pixelsWide:rayTracer.width
                                                                            pixelsHigh:rayTracer.height
                                                                         bitsPerSample:8
                                                                       samplesPerPixel:3
                                                                              hasAlpha:NO
                                                                              isPlanar:NO
                                                                        colorSpaceName:NSCalibratedRGBColorSpace
                                                                           bytesPerRow:0
                                                                          bitsPerPixel:0];
    
    for (int i = 0; i < (rayTracer.width * rayTracer.height); i++) {
        Color c = buffer->pixels[i];
        NSColor *color = [NSColor colorWithCalibratedRed:c.r green:c.g blue:c.b alpha:1.0];
        int y = i / rayTracer.width;
        int x = i - (y * rayTracer.width);
        [bitmapImage setColor:color atX:x y:rayTracer.height-y];
    }
    
    NSImage *finalImage = [[NSImage alloc] initWithCGImage:bitmapImage.CGImage size:bitmapImage.size];
    self.imageView.image = finalImage;
}


- (IBAction)saveDocument:(id)sender
{
    NSSavePanel *savePanel = [NSSavePanel savePanel];
    savePanel.allowedFileTypes = @[@"png"];
    
    [savePanel beginWithCompletionHandler:^(NSModalResponse result) {
        if (result == NSModalResponseOK) {
            NSImage *image = self.imageView.image;
            NSData *imageData = image.TIFFRepresentation;
            NSBitmapImageRep *imageRep = [NSBitmapImageRep imageRepWithData:imageData];
            NSDictionary *imageProps = [NSDictionary dictionaryWithObject:[NSNumber numberWithFloat:1.0] forKey:NSImageCompressionFactor];
            imageData = [imageRep representationUsingType:NSPNGFileType properties:imageProps];
            NSURL*  theFile = [savePanel URL];
            [imageData writeToURL:theFile atomically:YES];
        }
    }];
}

- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];

    // Update the view, if already loaded.
}


@end

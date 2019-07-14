//
//  ViewController.m
//  raytracer
//
//  Created by Adrian Russell on 29/06/2018.
//  Copyright © 2018 Adrian Russell. All rights reserved.
//

#import "ViewController.h"
#include "Integrator.hpp"
#include "test_scenes.h"

@implementation ViewController
{
    ImageBuffer *_outputBuffer;
    NSTimer *_timer;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
    
    int width = 500;
    int height = 500;
    int ns = 100;
    
    Scene *s = cornellBoxWithSphere();
    //Scene *s = legoMan();
    //Scene *s = glass();
    //scene *s = phongSpheresScene();
    //Scene *s = rbgLightSpheres();
    //Scene *s = cornellBoxWithSuzanne();
    ImageBuffer *buffer = new ImageBuffer(width, height);
    _outputBuffer = buffer;
    Integrator *integrator = new TestIntegrator2(buffer, ns);
    
    dispatch_async(dispatch_get_global_queue(QOS_CLASS_USER_INITIATED, 0), ^{
        integrator->preprocess(*s);
        integrator->render(*s);
        
        dispatch_async(dispatch_get_main_queue(), ^{
            [self->_timer invalidate];
            [self updateImageView];
        });
    });
    
    
    _timer = [NSTimer scheduledTimerWithTimeInterval:2.0 target:self selector:@selector(updateImageView) userInfo:nil repeats:YES];
}

- (void)updateImageView
{
    ImageBuffer *buffer = _outputBuffer;
    int width = _outputBuffer->width;
    int height = _outputBuffer->height;
    
    NSBitmapImageRep *bitmapImage = [[NSBitmapImageRep alloc] initWithBitmapDataPlanes:NULL
                                                                            pixelsWide:width
                                                                            pixelsHigh:height
                                                                         bitsPerSample:8
                                                                       samplesPerPixel:3
                                                                              hasAlpha:NO
                                                                              isPlanar:NO
                                                                        colorSpaceName:NSCalibratedRGBColorSpace
                                                                           bytesPerRow:0
                                                                          bitsPerPixel:0];
    
    for (int i = 0; i < (width * height); i++) {
        Color c = buffer->pixels[i];
        NSColor *color = [NSColor colorWithCalibratedRed:c.r green:c.g blue:c.b alpha:1.0];
        int y = i / width;
        int x = i - (y * width);
        [bitmapImage setColor:color atX:x y:height-y];
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

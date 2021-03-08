#import <Cocoa/Cocoa.h>

#import "dtan.h"

#define FONT_TYPE @"YuMin-Demibold"

int DtanRun (DtanObject* dtanObject) {
    @autoreleasepool {
        // Application
        [NSApplication sharedApplication];
        [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];

        // Window
        NSRect screen = [[NSScreen deepestScreen] frame];
        id window = [
            [NSWindow alloc]
            initWithContentRect:NSMakeRect(0, 0, screen.size.width, screen.size.height)
            styleMask:NSWindowStyleMaskBorderless
            backing:NSBackingStoreBuffered
            defer:NO
        ];

        {
            // Name
            [window setTitle: [[NSProcessInfo processInfo] processName]];

            // Color & Effect
            [window setBackgroundColor:[NSColor clearColor]];
            [window setOpaque:NO];
            [window setHasShadow:false];

            // Position
            [window cascadeTopLeftFromPoint:NSMakePoint(0,0)];
            [window setCollectionBehavior:NSWindowCollectionBehaviorCanJoinAllSpaces | NSWindowCollectionBehaviorFullScreenAuxiliary];
            [window setLevel:NSFloatingWindowLevel];
            [window makeKeyAndOrderFront:nil];
        }

        // Text
        {
            // Font
            NSString* display_string = [
                [NSString stringWithCString: dtanObject->display_string encoding:NSUTF8StringEncoding]
                stringByReplacingOccurrencesOfString:@" " withString:@"\u00a0"
            ];

            // String
            NSTextField* text_field = [
                [NSTextField alloc]
                initWithFrame:NSMakeRect(0, screen.size.height/2-(FONT_SIZE/2), screen.size.width, FONT_SIZE*2)
            ];
            [text_field setFont:[NSFont fontWithName:FONT_TYPE size:FONT_SIZE]];
            [text_field setBezeled:NO];
            [text_field setEditable:NO];
            [text_field setSelectable:NO];
            [text_field setDrawsBackground:NO];
            [text_field setTextColor:[NSColor whiteColor]];
            [text_field setStringValue:display_string];
            [text_field setAlignment:NSTextAlignmentCenter];

            NSMutableAttributedString *attributed_string = [
                [text_field attributedStringValue]
                mutableCopy
            ];
            [
                attributed_string addAttribute:NSUnderlineStyleAttributeName value:[NSNumber numberWithInt:NSUnderlineStyleSingle]
                range:NSMakeRange(0, attributed_string.length)
            ];
            [text_field setAttributedStringValue:attributed_string];

            // Set Text Object
            id superview = [window contentView];
            [superview addSubview:text_field];

            id layout_x = [
                NSLayoutConstraint
                constraintWithItem:superview
                attribute:NSLayoutAttributeCenterX
                relatedBy:NSLayoutRelationEqual
                toItem:text_field
                attribute:NSLayoutAttributeCenterX
                multiplier:1.0
                constant:0.0
            ];

            id layout_y = [
                NSLayoutConstraint
                constraintWithItem:superview
                attribute:NSLayoutAttributeCenterY
                relatedBy:NSLayoutRelationEqual
                toItem:text_field
                attribute:NSLayoutAttributeCenterY
                multiplier:1.0
                constant:0.0
            ];

            [superview addConstraint:layout_x];
            [superview addConstraint:layout_y];
        }

        // Draw
        [NSApp activateIgnoringOtherApps:YES];
        [NSApp run];

        // Exit
        [NSApp terminate:nil];
    }

    return 0;
}

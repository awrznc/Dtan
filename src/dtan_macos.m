#import <Cocoa/Cocoa.h>

#import "dtan.h"

#define FONT_TYPE @"YuMin-Demibold"

@interface DtanApplication : NSApplication

{
    bool shouldKeepRunning;
    int count;
    int endcount;
    DtanObject* dtanObject;
}

- (void)update;
- (void)run;
- (void)terminate:(id)sender;

@end


@implementation DtanApplication

- (void)setDtanObject:(DtanObject*)originalDtanObject {
    dtanObject = originalDtanObject;
}

- (void)update {
    if( !DtanUpdateStatus(dtanObject) ) shouldKeepRunning = NO;
    for(NSWindow *window in [self windows]){
        for (NSView *view in window.contentView.subviews) {
            NSMutableAttributedString *neo_attributed_string = [
                [(NSTextField*)view attributedStringValue]
                mutableCopy
            ];
            [
                neo_attributed_string
                addAttribute:NSForegroundColorAttributeName
                value:[NSColor colorWithDeviceRed:0xff green:0xff blue:0xff alpha:((double)dtanObject->status.alpha)/100]
                range:NSMakeRange(0, neo_attributed_string.length)
            ];

            // Update text color
            [(NSTextField*)view setAttributedStringValue:neo_attributed_string];
            [window.contentView addSubview:(NSTextField*)view];
        }
    }
}

- (void)run {

    shouldKeepRunning = YES;

    while (shouldKeepRunning) {

        [self update];

        NSEvent *event = [
            self
            nextEventMatchingMask:NSEventMaskAny
            untilDate:[NSDate dateWithTimeIntervalSinceNow:(UPDATE_PERIOD/1000.0)]
            inMode:NSDefaultRunLoopMode
            dequeue:YES
        ];

        [self sendEvent:event];
        [self updateWindows];
    }
}

- (void)terminate:(id)sender {
    shouldKeepRunning = NO;
}

@end


int DtanRun (DtanObject* dtanObject) {
    @autoreleasepool {
        // Application
        DtanApplication *application = [DtanApplication sharedApplication];
        [application setDtanObject:dtanObject];
        [application setActivationPolicy:NSApplicationActivationPolicyRegular];

        // Window
        NSRect screen = [[NSScreen deepestScreen] frame];
        NSWindow* window = [
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
            // [text_field setTextColor:[NSColor colorWithDeviceRed:0xff green:0xff blue:0xff alpha:1.0]];
            [text_field setStringValue:display_string];
            [text_field setAlignment:NSTextAlignmentCenter];

            NSMutableAttributedString *attributed_string = [
                [text_field attributedStringValue]
                mutableCopy
            ];
            [
                attributed_string
                addAttribute:NSUnderlineStyleAttributeName
                value:[NSNumber numberWithInt:NSUnderlineStyleSingle]
                range:NSMakeRange(0, attributed_string.length)
            ];
            [
                attributed_string
                addAttribute:NSForegroundColorAttributeName
                value:[NSColor colorWithDeviceRed:0xff green:0xff blue:0xff alpha:((double)dtanObject->status.alpha)/100]
                range:NSMakeRange(0, attributed_string.length)
            ];
            [text_field setAttributedStringValue:attributed_string];

            // Set Text Object
            NSView* superview = [window contentView];
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
        [application activateIgnoringOtherApps:YES];
        [application run];

        // Exit
        [application terminate:nil];
    }

    return 0;
}

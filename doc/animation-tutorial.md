<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->
# Contents

- [Slate - Animation Tutorial](#slate---animation-tutorial)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

# Slate - Animation Tutorial

In Slate, it's possible to preview animations. Each animation is based on one or more "frames" in your image. This tutorial will show one way to create these frames and then go through the process of customising the animation settings.

Firstly, we'll start off with a single frame, composed of three layers:

![Single frame](https://github.com/mitchcurtis/slate/blob/release/doc/images/slate-animation-tutorial-1.png)

The initial project can be downloaded [here](https://github.com/mitchcurtis/slate/blob/release/tests/manual/screenshots/resources/animation-tutorial-1.slp).

We want to have 6 frames in our animation, so we need to increase the size of the canvas. To do so, open the Change Canvas Size dialog by clicking on the tool button in the left-hand side of the toolbar:

![Change Canvas Size](https://github.com/mitchcurtis/slate/blob/release/doc/images/slate-animation-tutorial-1.1.png)

Change the width of the image to 216 pixels and click OK to confirm. Now you should see this:

![New Canvas Size](https://github.com/mitchcurtis/slate/blob/release/doc/images/slate-animation-tutorial-2.png)

Let's make the transparent background white. Select the "background" layer in the Layers panel. Switch to the Fill tool, and the click on the transparent area to fill it with white. It will look then like this:

![Fill](https://github.com/mitchcurtis/slate/blob/release/doc/images/slate-animation-tutorial-2.1.png)

To make it easier to see which frame is which, we'll add some vertical guides between each one.

In order to drag a guide onto the canvas, the rulers must be visible. Click on the "Show rulers" tool button in the tool bar. Do the same for guides by clicking on the next tool button to the right.

We're going to recreate the following guides:

![Guides](https://github.com/mitchcurtis/slate/blob/release/doc/images/slate-animation-tutorial-2.2.png)

To drag a vertical guide onto the canvas, click on the ruler on the left side of the screen and drag outwards. Repeat this step until there are five vertical guides on the canvas at 36 pixel intervals. Then, drag a horizontal guide onto the canvas so that we have some reference for vertical movement in our animation.

Now we'll set up the animation. To enable animation playback for a project, either press the A key or open the Animation menu and check "Animation Playback". The Animation panel will then become visible.

![Animation Panel](https://github.com/mitchcurtis/slate/blob/release/doc/images/slate-animation-tutorial-3.png)

Note: to save having to recreate the animation itself, you can download the project file [here](https://github.com/mitchcurtis/slate/blob/release/tests/manual/screenshots/resources/animation-tutorial-3.slp) to skip to the end result.

Let's make more room for the Animation panel. Collapse the Layers panel by clicking the button in its top-right corner.

We'll now increase the size of the animation preview to make it easier to see. Click on the settings icon in the top right of the Animation panel to open the Animation Rreview Settings dialog. Increase the scale to 4, and then click OK:

![Animation Settings](https://github.com/mitchcurtis/slate/blob/release/doc/images/slate-animation-tutorial-3-1.png)

Now we'll set up the animation. Click on the configure button next to "Animation 1" in the Animation panel to open the Animation Settings dialog. Input the following values:

![Animation Settings](https://github.com/mitchcurtis/slate/blob/release/doc/images/slate-animation-tutorial-3-2.png)

Together, the "Frame X" and "Frame Y" form the starting point of the animation. We set them both to 0 here, but in a project with several different animations (such as a character in an idle animation, attacking animation, etc.), they would be set to different values to test out the different animations.

The "Frame Width" and "Frame Height" fields store the size of each frame.

The "Frame Count" controls how many frames are in this animation.

The "FPS", or Frames Per Second, controls the speed at which the animation plays.

Click OK to confirm the settings. Hit the play button to see the animation:

![Animation](https://github.com/mitchcurtis/slate/blob/release/doc/images/slate-animation-tutorial.gif)

It's also possible to export the animation as a GIF using the export button in the lower right of the animation panel. Note that transparency is not currently supported.

The final project can be downloaded [here](https://github.com/mitchcurtis/slate/blob/release/tests/manual/screenshots/resources/animation-tutorial-3.slp).

# CellApp
CellApp is a graphical cancer cell segmentation and analysis tool.

### Usage
- File -> Open Image
- Tools -> Process
- Optional: click to remove cells
- File -> Save Results

### Options
 * Filtering method
   * None: no filtering.
   * Gaussian: 9x9 filter that smoothens the image.
   * Bilateral: filter that keeps the edges but smoothens similar regions.
 * Threshold method
   * Color based: threshold using brown color.
   * Otsu's method: threshold using all colors.
 * Use watershed: use watershed algorithm to segment regions.
 * Use convex hull: ignore curves that are bulged inside.
 * Minimum cell area: cells smaller than this will be ignored.
 * Save analysis results: save analysis results as text files when using Process All.
 * Save segmented image: save segmented images when using Process All.

### Output parameters
 * Area: cell area.
 * Perimeter: cell perimeter.
 * Distance: distance from the largest cell.
 * Ratio: cell rounding rectangle longer side / shorter side.

### Licence
Copyright (C) 2018 [Eero Molkoselkä](mailto:eero.molkoselka@gmail.com)

CellApp is licensed under WTFPL. See COPYING file for details.

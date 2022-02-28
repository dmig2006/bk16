#ifndef VIEW_ENUM_H
#define VIEW_ENUM_H

// Video Frame Resolution -----------------------------------------------------
#define DEFAULT_WIDTH   768
#define DEFAULT_HEIGHT  576

// Pix to Motor QC ------------------------------------------------------------
#define PIX_TO_QC_WIDE_H            3.350
#define PIX_TO_QC_WIDE_W            14.31 
#define PIX_TO_QC_NARROW_H          0.569
#define PIX_TO_QC_NARROW_W          2.418

#define PIX_TO_QC_HEAT_H            1.450
#define PIX_TO_QC_HEAT_W            6.246
#define PIX_TO_QC_ZOOM_THERMAL_H    0.727
#define PIX_TO_QC_ZOOM_THERMAL_W    3.120

// Motor QC to Pix ------------------------------------------------------------
#define QC_TO_PIX_WIDE_H            0.298
#define QC_TO_PIX_WIDE_W            0.070
#define QC_TO_PIX_NARROW_H          1.760
#define QC_TO_PIX_NARROW_W          0.414

#define QC_TO_PIX_HEAT_H            0.688
#define QC_TO_PIX_HEAT_W            0.160
#define QC_TO_PIX_ZOOM_THERMAL_H    1.376
#define QC_TO_PIX_ZOOM_THERMAL_W    0.320

// Narrow Angle of View -------------------------------------------------------
#define ANGLE_NARROW_X          2.4
#define ANGLE_NARROW_Y          1.8

// Wide Angle of View ---------------------------------------------------------
#define ANGLE_WIDE_X            14.2
#define ANGLE_WIDE_Y            10.6

// Thermal Imager
#define ANGLE_THERMAL_X         6.2
#define ANGLE_THERMAL_Y         4.6
#define ANGLE_ZOOM_THERMAL_X    3.1
#define ANGLE_ZOOM_THERMAL_Y    2.3

// Video Modes ----------------------------------------------------------------
enum CameraMode {
    NARROW_FORMAT = 1,
    WIDE_FORMAT   = 2,
    THERMAL       = 3,
    ZOOM_THERMAL  = 4
};

#endif // VIEW_ENUM_H

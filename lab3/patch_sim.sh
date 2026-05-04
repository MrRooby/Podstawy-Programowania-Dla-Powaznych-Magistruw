#!/bin/bash
patch -p0 << 'PATCH'
--- src/SimulationController.cpp
+++ src/SimulationController.cpp
@@ -13,16 +13,12 @@
 }
 
 void SimulationController::setControllerParams(double k, double ti, double td) {
-    if (controller) {
-        controller->set_k(k);
-        controller->set_ti(ti);
-        controller->set_td(td);
-    }
+    if (controller) delete controller;
+    controller = new ControllerPID(k, ti, td);
 }
 
 void SimulationController::setModelParams(double a1, double b0, int delay, double noise_std) {
-    if (model) {
-        delete model;
-        model = new ModelArx({1.0, a1}, {b0}, delay, noise_std);
-    }
+    if (model) delete model;
+    model = new ModelArx({1.0, a1}, {b0}, delay, noise_std);
 }
 
PATCH

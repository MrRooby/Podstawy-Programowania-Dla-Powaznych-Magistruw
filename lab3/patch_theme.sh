#!/bin/bash
patch -p0 << 'PATCH'
--- src/MainWindow.cpp
+++ src/MainWindow.cpp
@@ -345,63 +345,63 @@
 void MainWindow::applyTheme(bool dark) {
     if (dark) {
         qApp->setStyleSheet(
-            "QWidget { background-color: #1e1e1e; color: #f5f5f7; font-family: -apple-system, 'Segoe UI', sans-serif; font-size: 13px; }"
-            "QPushButton { background-color: #333333; border: 1px solid #444444; border-radius: 6px; padding: 6px 14px; font-weight: bold; color: #f5f5f7; }"
-            "QPushButton:hover { background-color: #404040; }"
-            "QPushButton:pressed { background-color: #4d4d4d; }"
-            
-            "QPushButton#btnRun { background-color: #1b4527; border: 1px solid #235c34; color: #4cd964; }"
-            "QPushButton#btnRun:hover { background-color: #235c34; }"
-            "QPushButton#btnRun:pressed { background-color: #2a7341; }"
-            
-            "QPushButton#btnReset { background-color: #5c1e1a; border: 1px solid #7a2822; color: #ff3b30; }"
-            "QPushButton#btnReset:hover { background-color: #7a2822; }"
-            "QPushButton#btnReset:pressed { background-color: #943029; }"
-            
-            "QPushButton#btnAddSignal, QPushButton#btnSave { background-color: #1a3d66; border: 1px solid #225188; color: #64d2ff; }"
-            "QPushButton#btnAddSignal:hover, QPushButton#btnSave:hover { background-color: #225188; }"
-            "QPushButton#btnAddSignal:pressed, QPushButton#btnSave:pressed { background-color: #2a65a6; }"
-            
-            "QPushButton#btnClearSignals { background-color: #5c3d1a; border: 1px solid #7a5122; color: #ff9f0a; }"
-            "QPushButton#btnClearSignals:hover { background-color: #7a5122; }"
-            "QPushButton#btnClearSignals:pressed { background-color: #946229; }"
-            
-            "QGroupBox { background-color: #252525; border: 1px solid #333333; border-radius: 8px; margin-top: 16px; padding-top: 12px; }"
-            "QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top left; left: 12px; top: 0px; color: #98989d; font-weight: bold; }"
-            "QComboBox, QSpinBox, QDoubleSpinBox { background-color: #2c2c2e; border: 1px solid #3a3a3c; border-radius: 6px; padding: 5px; color: #f5f5f7; }"
-            "QComboBox::drop-down { border: none; width: 22px; }"
-            "QListWidget { background-color: #252525; border: 1px solid #333333; border-radius: 8px; padding: 4px; }"
-            "QScrollBar:vertical { border: none; background: transparent; width: 12px; margin: 0px; }"
-            "QScrollBar::handle:vertical { background: #48484a; min-height: 20px; border-radius: 6px; }"
+            "QWidget { background-color: #282828; color: #ebdbb2; font-family: -apple-system, 'Segoe UI', sans-serif; font-size: 13px; }"
+            "QPushButton { background-color: #3c3836; border: 1px solid #504945; border-radius: 6px; padding: 6px 14px; font-weight: bold; color: #ebdbb2; }"
+            "QPushButton:hover { background-color: #504945; }"
+            "QPushButton:pressed { background-color: #665c54; }"
+            
+            "QPushButton#btnRun { background-color: #32302f; border: 1px solid #98971a; color: #b8bb26; }"
+            "QPushButton#btnRun:hover { background-color: #3c3836; }"
+            "QPushButton#btnRun:pressed { background-color: #504945; }"
+            
+            "QPushButton#btnReset { background-color: #32302f; border: 1px solid #cc241d; color: #fb4934; }"
+            "QPushButton#btnReset:hover { background-color: #3c3836; }"
+            "QPushButton#btnReset:pressed { background-color: #504945; }"
+            
+            "QPushButton#btnAddSignal, QPushButton#btnSave { background-color: #32302f; border: 1px solid #458588; color: #83a598; }"
+            "QPushButton#btnAddSignal:hover, QPushButton#btnSave:hover { background-color: #3c3836; }"
+            "QPushButton#btnAddSignal:pressed, QPushButton#btnSave:pressed { background-color: #504945; }"
+            
+            "QPushButton#btnClearSignals { background-color: #32302f; border: 1px solid #d79921; color: #fabd2f; }"
+            "QPushButton#btnClearSignals:hover { background-color: #3c3836; }"
+            "QPushButton#btnClearSignals:pressed { background-color: #504945; }"
+            
+            "QGroupBox { background-color: #1d2021; border: 1px solid #504945; border-radius: 8px; margin-top: 16px; padding-top: 12px; }"
+            "QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top left; left: 12px; top: 0px; color: #a89984; font-weight: bold; }"
+            "QComboBox, QSpinBox, QDoubleSpinBox { background-color: #3c3836; border: 1px solid #504945; border-radius: 6px; padding: 5px; color: #ebdbb2; }"
+            "QComboBox::drop-down { border: none; width: 22px; }"
+            "QListWidget { background-color: #1d2021; border: 1px solid #504945; border-radius: 8px; padding: 4px; }"
+            "QScrollBar:vertical { border: none; background: transparent; width: 12px; margin: 0px; }"
+            "QScrollBar::handle:vertical { background: #504945; min-height: 20px; border-radius: 6px; }"
         );
     } else {
         qApp->setStyleSheet(
-            "QWidget { background-color: #f5f5f7; color: #1d1d1f; font-family: -apple-system, 'Segoe UI', sans-serif; font-size: 13px; }"
-            "QPushButton { background-color: #ffffff; border: 1px solid #d1d1d6; border-radius: 6px; padding: 6px 14px; font-weight: bold; color: #1d1d1f; }"
-            "QPushButton:hover { background-color: #f0f0f0; }"
-            "QPushButton:pressed { background-color: #e5e5ea; }"
-            
-            "QPushButton#btnRun { background-color: #e4f8ea; border: 1px solid #d4f3e0; color: #248a3d; }"
-            "QPushButton#btnRun:hover { background-color: #d4f3e0; }"
-            "QPushButton#btnRun:pressed { background-color: #c4eed0; }"
-            
-            "QPushButton#btnReset { background-color: #ffe5e5; border: 1px solid #ffd4d4; color: #c9342c; }"
-            "QPushButton#btnReset:hover { background-color: #ffd4d4; }"
-            "QPushButton#btnReset:pressed { background-color: #ffc4c4; }"
-            
-            "QPushButton#btnAddSignal, QPushButton#btnSave { background-color: #e5f1ff; border: 1px solid #d4e8ff; color: #0056b3; }"
-            "QPushButton#btnAddSignal:hover, QPushButton#btnSave:hover { background-color: #d4e8ff; }"
-            "QPushButton#btnAddSignal:pressed, QPushButton#btnSave:pressed { background-color: #c2ddff; }"
-            
-            "QPushButton#btnClearSignals { background-color: #fff0e5; border: 1px solid #ffe4d4; color: #b36b00; }"
-            "QPushButton#btnClearSignals:hover { background-color: #ffe4d4; }"
-            "QPushButton#btnClearSignals:pressed { background-color: #ffd7c2; }"
-            
-            "QGroupBox { background-color: #ffffff; border: 1px solid #e5e5ea; border-radius: 8px; margin-top: 16px; padding-top: 12px; }"
-            "QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top left; left: 12px; top: 0px; color: #86868b; font-weight: bold; }"
-            "QComboBox, QSpinBox, QDoubleSpinBox { background-color: #ffffff; border: 1px solid #d1d1d6; border-radius: 6px; padding: 5px; color: #1d1d1f; }"
-            "QComboBox::drop-down { border: none; width: 22px; }"
-            "QListWidget { background-color: #ffffff; border: 1px solid #e5e5ea; border-radius: 8px; padding: 4px; }"
-            "QScrollBar:vertical { border: none; background: transparent; width: 12px; margin: 0px; }"
-            "QScrollBar::handle:vertical { background: #d1d1d6; min-height: 20px; border-radius: 6px; }"
+            "QWidget { background-color: #fbf1c7; color: #3c3836; font-family: -apple-system, 'Segoe UI', sans-serif; font-size: 13px; }"
+            "QPushButton { background-color: #ebdbb2; border: 1px solid #d5c4a1; border-radius: 6px; padding: 6px 14px; font-weight: bold; color: #3c3836; }"
+            "QPushButton:hover { background-color: #d5c4a1; }"
+            "QPushButton:pressed { background-color: #bdae93; }"
+            
+            "QPushButton#btnRun { background-color: #f2e5bc; border: 1px solid #98971a; color: #79740e; }"
+            "QPushButton#btnRun:hover { background-color: #ebdbb2; }"
+            "QPushButton#btnRun:pressed { background-color: #d5c4a1; }"
+            
+            "QPushButton#btnReset { background-color: #f2e5bc; border: 1px solid #cc241d; color: #9d0006; }"
+            "QPushButton#btnReset:hover { background-color: #ebdbb2; }"
+            "QPushButton#btnReset:pressed { background-color: #d5c4a1; }"
+            
+            "QPushButton#btnAddSignal, QPushButton#btnSave { background-color: #f2e5bc; border: 1px solid #458588; color: #076678; }"
+            "QPushButton#btnAddSignal:hover, QPushButton#btnSave:hover { background-color: #ebdbb2; }"
+            "QPushButton#btnAddSignal:pressed, QPushButton#btnSave:pressed { background-color: #d5c4a1; }"
+            
+            "QPushButton#btnClearSignals { background-color: #f2e5bc; border: 1px solid #d79921; color: #b57614; }"
+            "QPushButton#btnClearSignals:hover { background-color: #ebdbb2; }"
+            "QPushButton#btnClearSignals:pressed { background-color: #d5c4a1; }"
+            
+            "QGroupBox { background-color: #f9f5d7; border: 1px solid #d5c4a1; border-radius: 8px; margin-top: 16px; padding-top: 12px; }"
+            "QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top left; left: 12px; top: 0px; color: #7c6f64; font-weight: bold; }"
+            "QComboBox, QSpinBox, QDoubleSpinBox { background-color: #ebdbb2; border: 1px solid #d5c4a1; border-radius: 6px; padding: 5px; color: #3c3836; }"
+            "QComboBox::drop-down { border: none; width: 22px; }"
+            "QListWidget { background-color: #f9f5d7; border: 1px solid #d5c4a1; border-radius: 8px; padding: 4px; }"
+            "QScrollBar:vertical { border: none; background: transparent; width: 12px; margin: 0px; }"
+            "QScrollBar::handle:vertical { background: #d5c4a1; min-height: 20px; border-radius: 6px; }"
         );
     }
PATCH

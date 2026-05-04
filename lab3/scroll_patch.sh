#!/bin/bash
patch -p0 << 'PATCH'
--- src/MainWindow.cpp
+++ src/MainWindow.cpp
@@ -279,7 +279,12 @@
     
     QWidget* leftWidget = new QWidget();
     leftWidget->setLayout(leftLayout);
-    leftWidget->setMaximumWidth(280);
+    
+    QScrollArea* scrollArea = new QScrollArea();
+    scrollArea->setWidgetResizable(true);
+    scrollArea->setWidget(leftWidget);
+    scrollArea->setMinimumWidth(280);
+    scrollArea->setMaximumWidth(320);
+    scrollArea->setFrameShape(QFrame::NoFrame);
     
     // ===== RIGHT PANEL: Graphs =====
     QVBoxLayout* rightLayout = new QVBoxLayout();
@@ -303,7 +308,7 @@
     rightWidget->setLayout(rightLayout);
     
     // Main layout
-    mainLayout->addWidget(leftWidget, 0);
+    mainLayout->addWidget(scrollArea, 0);
     mainLayout->addWidget(rightWidget, 1);
 }
 
PATCH

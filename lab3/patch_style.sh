#!/bin/bash
patch -p1 << 'PATCH'
--- src/MainWindow.cpp
+++ src/MainWindow.cpp
@@ -40,11 +40,11 @@
 
 void GraphWidget::paintEvent(QPaintEvent* event) {
     QPainter painter(this);
-    painter.fillRect(rect(), isDarkMode ? QColor("#2b2b2b") : Qt::white);
+    painter.fillRect(rect(), isDarkMode ? QColor("#282828") : QColor("#fbf1c7"));
     
-    painter.setPen(isDarkMode ? Qt::white : Qt::black);
+    painter.setPen(isDarkMode ? QColor("#ebdbb2") : QColor("#3c3836"));
     
     if (xData.empty() || yData.empty()) {
         painter.drawText(rect(), Qt::AlignCenter, "No data to display");
         return;
     }
@@ -83,14 +83,14 @@
     painter.drawLine(margin, height()-margin, width()-margin, height()-margin);
     
     // Draw grid and labels
-    QColor gridColor = isDarkMode ? QColor("#555") : Qt::lightGray;
+    QColor gridColor = isDarkMode ? QColor("#504945") : QColor("#d5c4a1");
     painter.setPen(QPen(gridColor, 1, Qt::DashLine));
     for (int i = 0; i <= 5; ++i) {
         int y = margin + (plotH * i) / 5;
         painter.drawLine(margin, y, width()-margin, y);
         double val = yMax - (yMax - yMin) * i / 5;
-        painter.setPen(isDarkMode ? Qt::white : Qt::black);
+        painter.setPen(isDarkMode ? QColor("#ebdbb2") : QColor("#3c3836"));
         painter.drawText(5, y+3, QString::number(val, 'f', 2));
         painter.setPen(QPen(gridColor, 1, Qt::DashLine));
     }
     
     // Draw data lines
-    painter.setPen(QPen(isDarkMode ? QColor("#4da6ff") : Qt::blue, 2));
+    painter.setPen(QPen(isDarkMode ? QColor("#83a598") : QColor("#076678"), 2));
     for (size_t i = 1; i < xData.size() && i < yData.size(); ++i) {
         double x1 = margin + plotW * (xData[i-1] - xMin) / (xMax - xMin);
@@ -104,11 +104,11 @@
     
     // Draw second line if present
     if (!yData2.empty()) {
-        painter.setPen(QPen(isDarkMode ? QColor("#ff4d4d") : Qt::red, 2));
+        painter.setPen(QPen(isDarkMode ? QColor("#fb4934") : QColor("#9d0006"), 2));
         for (size_t i = 1; i < xData.size() && i < yData2.size(); ++i) {
             double x1 = margin + plotW * (xData[i-1] - xMin) / (xMax - xMin);
PATCH

    update();
}

void GraphWidget::setData(const std::vector<double>& x, const std::vector<double>& y,
                          const std::vector<double>& y2) {
    xData = x;
    yData = y;
    yData2 = y2;
    update();
}

void GraphWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.fillRect(rect(), isDarkMode ? QColor("#2b2b2b") : Qt::white);
    
    painter.setPen(isDarkMode ? Qt::white : Qt::black);
    
    if (xData.empty() || yData.empty()) {
        painter.drawText(rect(), Qt::AlignCenter, "No data to display");
        return;
    }
    
    // Draw border
    painter.drawRect(0, 0, width()-1, height()-1);
    
    // Draw title
    painter.setFont(QFont("Arial", 10, QFont::Bold));
    painter.drawText(5, 15, title);
    
    // Calculate data bounds
    double xMin = *std::min_element(xData.begin(), xData.end());
    double xMax = *std::max_element(xData.begin(), xData.end());
    double yMin = *std::min_element(yData.begin(), yData.end());
    double yMax = *std::max_element(yData.begin(), yData.end());
    
    if (!yData2.empty()) {
        yMin = std::min(yMin, *std::min_element(yData2.begin(), yData2.end()));
        yMax = std::max(yMax, *std::max_element(yData2.begin(), yData2.end()));
    }
    
    // Add padding
    double yPad = (yMax - yMin) * 0.1;
    if (yPad == 0) yPad = 0.5;
    yMin -= yPad;
    yMax += yPad;
    
    int margin = 40;
    int plotW = width() - 2*margin;
    int plotH = height() - 2*margin - 20;
    
    // Draw axes
    painter.setFont(QFont("Arial", 8));
    painter.drawLine(margin, margin, margin, height()-margin);
    painter.drawLine(margin, height()-margin, width()-margin, height()-margin);
    
    // Draw grid and labels
    QColor gridColor = isDarkMode ? QColor("#555") : Qt::lightGray;
    painter.setPen(QPen(gridColor, 1, Qt::DashLine));
    for (int i = 0; i <= 5; ++i) {
        int y = margin + (plotH * i) / 5;
        painter.drawLine(margin, y, width()-margin, y);
        double val = yMax - (yMax - yMin) * i / 5;
        painter.setPen(isDarkMode ? Qt::white : Qt::black);
        painter.drawText(5, y+3, QString::number(val, 'f', 2));
        painter.setPen(QPen(gridColor, 1, Qt::DashLine));
    }
    
    // Draw data lines
    painter.setPen(QPen(isDarkMode ? QColor("#4da6ff") : Qt::blue, 2));
    for (size_t i = 1; i < xData.size() && i < yData.size(); ++i) {
        double x1 = margin + plotW * (xData[i-1] - xMin) / (xMax - xMin);
        double y1 = height()-margin - plotH * (yData[i-1] - yMin) / (yMax - yMin);
        double x2 = margin + plotW * (xData[i] - xMin) / (xMax - xMin);
        double y2 = height()-margin - plotH * (yData[i] - yMin) / (yMax - yMin);
        painter.drawLine(x1, y1, x2, y2);
    }
    
    // Draw second line if present
    if (!yData2.empty()) {
        painter.setPen(QPen(isDarkMode ? QColor("#ff4d4d") : Qt::red, 2));
        for (size_t i = 1; i < xData.size() && i < yData2.size(); ++i) {
            double x1 = margin + plotW * (xData[i-1] - xMin) / (xMax - xMin);
            double y1 = height()-margin - plotH * (yData2[i-1] - yMin) / (yMax - yMin);
            double x2 = margin + plotW * (xData[i] - xMin) / (xMax - xMin);
            double y2 = height()-margin - plotH * (yData2[i] - yMin) / (yMax - yMin);
            painter.drawLine(x1, y1, x2, y2);
        }
    }
}

// ===== MainWindow Implementation =====

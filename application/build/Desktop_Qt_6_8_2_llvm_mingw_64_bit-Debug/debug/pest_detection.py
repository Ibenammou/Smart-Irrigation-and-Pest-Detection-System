import cv2
import tensorflow as tf
import numpy as np
import sys
from PyQt5.QtWidgets import QApplication, QLabel, QVBoxLayout, QWidget
from PyQt5.QtCore import QTimer, Qt
from PyQt5.QtGui import QImage, QPixmap

class PestDetectionWindow(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Détection des parasites")
        self.setGeometry(100, 100, 800, 600)

        # Load your trained model
        self.model = tf.keras.models.load_model('keras_model.h5')

        # Camera setup
        self.cap = cv2.VideoCapture(0)
        if not self.cap.isOpened():
            print("Erreur: Impossible d'ouvrir la caméra")
            sys.exit(1)

        # UI setup
        self.image_label = QLabel(self)
        self.image_label.setAlignment(Qt.AlignCenter)
        self.result_label = QLabel("Analyse en cours...", self)
        self.result_label.setAlignment(Qt.AlignCenter)

        layout = QVBoxLayout()
        layout.addWidget(self.image_label)
        layout.addWidget(self.result_label)
        self.setLayout(layout)

        # Timer for frame updates
        self.timer = QTimer(self)
        self.timer.timeout.connect(self.update_frame)
        self.timer.start(30)  # Update every 30ms

    def update_frame(self):
        ret, frame = self.cap.read()
        if not ret:
            return

        # Preprocess frame for model
        resized = cv2.resize(frame, (224, 224))
        normalized = resized / 255.0
        input_array = np.expand_dims(normalized, axis=0)

        # Run detection
        predictions = self.model.predict(input_array)
        class_idx = np.argmax(predictions[0])
        confidence = predictions[0][class_idx]

        # Display results
        classes = ["Sain", "Parasite détecté"]
        result_text = f"{classes[class_idx]} (Confiance: {confidence:.2%})"
        self.result_label.setText(result_text)

        # Draw bounding box if pest detected
        if class_idx == 1:
            cv2.putText(frame, "PARASITE!", (50, 50),
                       cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2)

        # Convert to QImage and display
        rgb_image = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        h, w, ch = rgb_image.shape
        bytes_per_line = ch * w
        qt_image = QImage(rgb_image.data, w, h, bytes_per_line, QImage.Format_RGB888)
        self.image_label.setPixmap(QPixmap.fromImage(qt_image).scaled(
            self.image_label.width(), self.image_label.height(),
            Qt.KeepAspectRatio))

    def closeEvent(self, event):
        self.cap.release()
        event.accept()

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = PestDetectionWindow()
    window.show()
    sys.exit(app.exec_())

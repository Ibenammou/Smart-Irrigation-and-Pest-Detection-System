import cv2
import numpy as np
import sys
import os
from PyQt5.QtWidgets import QApplication, QLabel, QVBoxLayout, QWidget, QPushButton, QFileDialog, QMessageBox, QComboBox
from PyQt5.QtCore import QTimer, Qt
from PyQt5.QtGui import QImage, QPixmap
from ultralytics import YOLO

class AgritechDetectionWindow(QWidget):
    def __init__(self, model_path="best_agritech.pt"):
        super().__init__()
        self.setWindowTitle("Détection Agritech avec YOLO")
        self.setGeometry(100, 100, 800, 600)
        self.model_path = model_path

        # Charger le modèle YOLO
        try:
            self.model = YOLO(self.model_path)
            print(f"Modèle YOLO chargé avec succès: {self.model_path}")

            # Classes prédéfinies pour le modèle
            self.class_names = ["pomme malade", "pomme saint", "orange malade",
                               "orange saint", "citron malade", "citron saint"]
            print(f"Classes chargées: {self.class_names}")
        except Exception as e:
            print(f"Erreur lors du chargement du modèle: {e}")
            QMessageBox.critical(self, "Erreur", f"Impossible de charger le modèle: {str(e)}")
            sys.exit(1)

        # Configuration de l'interface utilisateur
        self.setup_ui()

        # Variables pour la caméra
        self.using_camera = False
        self.cap = None
        self.timer = QTimer(self)
        self.timer.timeout.connect(self.update_frame)

        # Variable pour stocker l'image actuelle
        self.current_image = None

        # Scanner les caméras disponibles
        self.available_cameras = self.scan_cameras()
        self.populate_camera_dropdown()

    def setup_ui(self):
        """Configure l'interface utilisateur"""
        # Créer les widgets
        self.image_label = QLabel(self)
        self.image_label.setAlignment(Qt.AlignCenter)
        self.image_label.setMinimumSize(640, 480)

        self.result_label = QLabel("Prêt pour l'analyse", self)
        self.result_label.setAlignment(Qt.AlignCenter)
        self.result_label.setStyleSheet("font-size: 14pt;")

        # Sélection de caméra
        self.camera_dropdown_label = QLabel("Sélectionner une caméra:", self)
        self.camera_dropdown = QComboBox(self)

        # Boutons pour les sources d'entrée
        self.camera_button = QPushButton("Utiliser la caméra", self)
        self.camera_button.clicked.connect(self.start_camera)

        self.file_button = QPushButton("Choisir une image", self)
        self.file_button.clicked.connect(self.select_image)

        self.stop_camera_button = QPushButton("Arrêter la caméra", self)
        self.stop_camera_button.clicked.connect(self.stop_camera)
        self.stop_camera_button.setEnabled(False)

        self.refresh_cameras_button = QPushButton("Rafraîchir liste caméras", self)
        self.refresh_cameras_button.clicked.connect(self.refresh_cameras)

        # Organiser l'interface
        layout = QVBoxLayout()
        layout.addWidget(self.image_label)
        layout.addWidget(self.result_label)

        # Layout pour la sélection de caméra
        camera_select_layout = QVBoxLayout()
        camera_select_layout.addWidget(self.camera_dropdown_label)
        camera_select_layout.addWidget(self.camera_dropdown)
        camera_select_layout.addWidget(self.refresh_cameras_button)
        layout.addLayout(camera_select_layout)

        button_layout = QVBoxLayout()
        button_layout.addWidget(self.camera_button)
        button_layout.addWidget(self.stop_camera_button)
        button_layout.addWidget(self.file_button)
        layout.addLayout(button_layout)

        self.setLayout(layout)

    def scan_cameras(self):
        """Scanne les caméras disponibles et retourne un dictionnaire de {index: statut}"""
        available_cameras = {}

        # Vérifier si le système est Windows
        if os.name == 'nt':
            # Sur Windows, essayer DirectShow et Media Foundation
            backends = [cv2.CAP_DSHOW, cv2.CAP_MSMF, cv2.CAP_ANY]
        else:
            # Sur Linux/Mac, essayer V4L2 et d'autres
            backends = [cv2.CAP_V4L2, cv2.CAP_ANY]

        # Essayer de détecter jusqu'à 10 caméras
        for idx in range(10):
            camera_works = False
            working_backend = None

            for backend in backends:
                try:
                    temp_cap = cv2.VideoCapture(idx, backend)
                    if temp_cap.isOpened():
                        ret, frame = temp_cap.read()
                        if ret:
                            camera_works = True
                            working_backend = backend
                            print(f"Caméra {idx} détectée avec backend {backend}")
                        temp_cap.release()
                        if camera_works:
                            break
                except Exception as e:
                    print(f"Erreur lors du test de la caméra {idx} avec backend {backend}: {e}")

            if camera_works:
                backend_name = "Inconnu"
                if working_backend == cv2.CAP_DSHOW:
                    backend_name = "DirectShow"
                elif working_backend == cv2.CAP_MSMF:
                    backend_name = "Media Foundation"
                elif working_backend == cv2.CAP_V4L2:
                    backend_name = "V4L2"

                available_cameras[idx] = {
                    "status": "Disponible",
                    "backend": working_backend,
                    "backend_name": backend_name
                }
            else:
                # Si on a vérifié au moins 3 caméras sans en trouver, arrêter la recherche
                if idx >= 2 and not available_cameras:
                    break

        return available_cameras

    def populate_camera_dropdown(self):
        """Remplit le menu déroulant avec les caméras disponibles"""
        self.camera_dropdown.clear()

        if not self.available_cameras:
            self.camera_dropdown.addItem("Aucune caméra détectée")
            self.camera_button.setEnabled(False)
            return

        for idx, info in self.available_cameras.items():
            self.camera_dropdown.addItem(f"Caméra {idx} ({info['backend_name']})")

        self.camera_button.setEnabled(True)

    def refresh_cameras(self):
        """Rafraîchit la liste des caméras disponibles"""
        self.result_label.setText("Recherche des caméras...")
        QApplication.processEvents()  # Assure que l'interface est mise à jour

        self.stop_camera()  # Arrêter la caméra si elle est déjà utilisée
        self.available_cameras = self.scan_cameras()
        self.populate_camera_dropdown()

        if self.available_cameras:
            self.result_label.setText(f"{len(self.available_cameras)} caméra(s) trouvée(s)")
        else:
            self.result_label.setText("Aucune caméra détectée")

    def start_camera(self):
        """Essaye d'initialiser la caméra sélectionnée et commence la capture vidéo"""
        # Arrêter la caméra existante si elle est déjà active
        if self.using_camera and self.cap is not None:
            self.stop_camera()

        # Récupérer l'index de caméra sélectionné
        if not self.available_cameras:
            QMessageBox.warning(self, "Erreur", "Aucune caméra disponible")
            return

        selected_text = self.camera_dropdown.currentText()
        if "Aucune caméra" in selected_text:
            QMessageBox.warning(self, "Erreur", "Aucune caméra disponible")
            return

        # Extraire l'index de la caméra du texte sélectionné
        try:
            camera_idx = int(selected_text.split(' ')[1].split('(')[0])
            backend = self.available_cameras[camera_idx]["backend"]
        except:
            QMessageBox.warning(self, "Erreur", "Sélection de caméra invalide")
            return

        # Essayer d'ouvrir la caméra avec le backend spécifique
        try:
            print(f"Tentative d'ouverture de la caméra {camera_idx} avec backend {backend}...")

            # Solution 1: Utiliser le backend spécifique
            self.cap = cv2.VideoCapture(camera_idx, backend)

            # Solution 2: Essayer de réinitialiser les propriétés
            if not self.cap.isOpened():
                print("Essai avec réinitialisation des propriétés...")
                self.cap = cv2.VideoCapture(camera_idx)
                self.cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
                self.cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)
                self.cap.set(cv2.CAP_PROP_FPS, 30)

            # Solution 3: Essayer avec les paramètres de gdk (principalement pour Linux)
            if not self.cap.isOpened() and os.name != 'nt':
                print("Essai avec GDK...")
                os.environ["OPENCV_VIDEOIO_PRIORITY_GDK"] = "1"
                self.cap = cv2.VideoCapture(camera_idx)

            # Vérifier si la caméra est correctement ouverte
            if self.cap.isOpened():
                # Essayer de lire une frame pour confirmer le fonctionnement
                ret, frame = self.cap.read()
                if not ret:
                    raise Exception("Impossible de lire une image de la caméra")

                print(f"Caméra {camera_idx} ouverte avec succès")
                self.using_camera = True
                self.timer.start(30)  # 30ms refresh
                self.result_label.setText("Analyse en cours avec caméra...")
                self.camera_button.setEnabled(False)
                self.stop_camera_button.setEnabled(True)

                # Afficher la première frame
                self.display_image(frame)
                return
            else:
                raise Exception("La caméra ne peut pas être ouverte")

        except Exception as e:
            print(f"Erreur lors de l'ouverture de la caméra {camera_idx}: {e}")
            QMessageBox.warning(self, "Erreur de caméra",
                          f"Impossible d'accéder à la caméra {camera_idx}. Erreur: {str(e)}")
            self.cap = None
            self.using_camera = False

    def stop_camera(self):
        """Arrête la caméra"""
        if self.using_camera and self.cap is not None:
            self.timer.stop()
            self.cap.release()
            self.using_camera = False
            self.cap = None
            self.camera_button.setEnabled(True)
            self.stop_camera_button.setEnabled(False)
            self.result_label.setText("Caméra arrêtée")
            print("Caméra arrêtée")

    def select_image(self):
        """Ouvre un dialogue pour sélectionner une image et l'analyse"""
        file_path, _ = QFileDialog.getOpenFileName(self, "Sélectionner une image", "",
                                               "Images (*.png *.jpg *.jpeg)")

        if file_path:
            # Arrêter la caméra si elle est active
            if self.using_camera and self.cap is not None:
                self.stop_camera()

            # Charger et analyser l'image
            image = cv2.imread(file_path)
            if image is not None:
                self.current_image = image
                self.analyze_image(image)
            else:
                QMessageBox.warning(self, "Erreur", "Impossible de charger l'image.")

    def update_frame(self):
        """Met à jour le cadre de la caméra et fait l'analyse"""
        if not self.using_camera or self.cap is None:
            return

        ret, frame = self.cap.read()
        if not ret:
            print("Erreur lors de la capture d'image")
            # Essayer de lire encore une fois
            ret, frame = self.cap.read()
            if not ret:
                self.result_label.setText("Erreur de capture de la caméra")
                self.stop_camera()
                return

        self.current_image = frame.copy()
        self.analyze_image(frame)

    def analyze_image(self, frame):
        """Analyse une image avec le modèle YOLO"""
        try:
            # Exécuter la détection avec YOLO
            results = self.model(frame, stream=True)

            # Créer une copie de l'image pour l'affichage
            display_frame = frame.copy()

            # Analyser les résultats
            detections = []

            for r in results:
                boxes = r.boxes
                for box in boxes:
                    # Extraire les coordonnées de la boîte
                    x1, y1, x2, y2 = box.xyxy[0]
                    x1, y1, x2, y2 = int(x1), int(y1), int(x2), int(y2)

                    # Extraire la confiance
                    conf = float(box.conf[0])

                    # Extraire la classe
                    cls_idx = int(box.cls[0])
                    cls_name = self.class_names[cls_idx] if cls_idx < len(self.class_names) else f"Classe {cls_idx}"

                    # Stocker la détection
                    detections.append({"class": cls_name, "confidence": conf, "box": (x1, y1, x2, y2)})

                    # Dessiner le rectangle de détection
                    cv2.rectangle(display_frame, (x1, y1), (x2, y2), (255, 0, 255), 3)

                    # Afficher le nom de classe et la confiance
                    label = f"{cls_name} {conf:.2f}"
                    # Utiliser putTextRect de cvzone s'il est disponible, sinon utiliser putText normal
                    try:
                        import cvzone
                        cvzone.putTextRect(display_frame, label, (max(0, x1), max(35, y1)),
                                          scale=1, thickness=1)
                    except ImportError:
                        # Position du texte (en haut à gauche de la boîte)
                        y_pos = max(30, y1)
                        # Fond de texte pour meilleure lisibilité
                        (text_width, text_height), _ = cv2.getTextSize(label, cv2.FONT_HERSHEY_SIMPLEX,
                                                                     0.8, 2)
                        cv2.rectangle(display_frame, (x1, y_pos - text_height - 5),
                                    (x1 + text_width, y_pos + 5), (255, 0, 255), -1)
                        # Texte
                        cv2.putText(display_frame, label, (x1, y_pos),
                                   cv2.FONT_HERSHEY_SIMPLEX, 0.8, (255, 255, 255), 2)

            # Mettre à jour le texte de résultat
            if detections:
                result_text = "Détecté: " + ", ".join([f"{d['class']} ({d['confidence']:.2f})" for d in detections])
                self.result_label.setText(result_text)

                # Colorer le texte selon l'état détecté (malade/saint)
                if any("malade" in d["class"].lower() for d in detections):
                    self.result_label.setStyleSheet("font-size: 14pt; color: red; font-weight: bold;")
                else:
                    self.result_label.setStyleSheet("font-size: 14pt; color: green; font-weight: bold;")
            else:
                self.result_label.setText("Aucune détection")
                self.result_label.setStyleSheet("font-size: 14pt;")

            # Afficher l'image avec les détections
            self.display_image(display_frame)

        except Exception as e:
            print(f"Erreur pendant la détection YOLO: {e}")
            import traceback
            traceback.print_exc()
            self.result_label.setText(f"Erreur d'analyse: {str(e)[:50]}...")
            self.display_image(frame)

    def display_image(self, image):
        """Affiche une image dans le QLabel"""
        if image is not None and image.size > 0:  # Vérifier que l'image est valide
            try:
                rgb_image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
                h, w, ch = rgb_image.shape
                bytes_per_line = ch * w
                qt_image = QImage(rgb_image.data, w, h, bytes_per_line, QImage.Format_RGB888)
                self.image_label.setPixmap(QPixmap.fromImage(qt_image).scaled(
                    self.image_label.width(), self.image_label.height(),
                    Qt.KeepAspectRatio))
            except Exception as e:
                print(f"Erreur lors de l'affichage de l'image: {e}")
                import traceback
                traceback.print_exc()

    def closeEvent(self, event):
        """Gérer la fermeture de l'application"""
        if self.cap is not None and self.using_camera:
            self.cap.release()
        event.accept()

def main():
    """Fonction principale pour lancer l'application"""
    app = QApplication(sys.argv)

    # Définir des variables d'environnement pour OpenCV
    os.environ["OPENCV_VIDEOIO_PRIORITY_MSMF"] = "0"  # Désactiver Media Foundation en priorité
    os.environ["OPENCV_VIDEOIO_DEBUG"] = "1"  # Activer les logs de débug pour videoio

    # Déterminer le chemin du modèle à utiliser
    model_path = "best_agritech.pt"

    # Permettre de spécifier un autre modèle via les arguments de ligne de commande
    if len(sys.argv) > 1:
        model_path = sys.argv[1]

    window = AgritechDetectionWindow(model_path)
    window.show()
    sys.exit(app.exec_())

if __name__ == "__main__":
    main()

# apple_pest_detector.py
import numpy as np
import io
from PIL import Image, ImageOps
from keras.models import load_model

# Disable scientific notation for clarity
np.set_printoptions(suppress=True)

# Load the model and labels only once when the module is imported
try:
    model = load_model("keras_Model.h5", compile=False)
    class_names = open("labels.txt", "r").readlines()
    MODEL_LOADED = True
except Exception as e:
    print(f"Error loading model: {e}")
    MODEL_LOADED = False

def detect_pest(image_data):
    """
    Detect pests in an image provided as bytes.

    Args:
        image_data (bytes): The image data in bytes format (e.g., PNG, JPEG)

    Returns:
        tuple: (class_name, confidence_score)
    """
    if not MODEL_LOADED:
        return "Model not loaded", 0.0

    try:
        # Convert bytes to PIL Image
        image = Image.open(io.BytesIO(image_data)).convert("RGB")

        # Resize and preprocess the image
        size = (224, 224)
        image = ImageOps.fit(image, size, Image.Resampling.LANCZOS)

        # Convert to numpy array and normalize
        image_array = np.asarray(image)
        normalized_image_array = (image_array.astype(np.float32) / 127.5) - 1

        # Create input data array for the model
        data = np.ndarray(shape=(1, 224, 224, 3), dtype=np.float32)
        data[0] = normalized_image_array

        # Make prediction
        prediction = model.predict(data)
        index = np.argmax(prediction)
        class_name = class_names[index][2:].strip()  # Remove first 2 chars and whitespace
        confidence_score = float(prediction[0][index])

        return class_name, confidence_score

    except Exception as e:
        print(f"Error in pest detection: {e}")
        return f"Error: {str(e)}", 0.0

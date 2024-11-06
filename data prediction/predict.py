import tensorflow as tf
import numpy as np
import pandas as pd

def predict_waste(image_path, weight_model_path='normalized_weights.txt'):
    # Load the CNN model
    cnn_model = tf.keras.models.load_model('waste_classifier.h5')
    img = cv2.imread(image_path)
    img_resized = cv2.resize(img, (224, 224))
    img_array = np.expand_dims(img_resized, axis=0) / 255.0  # Normalize

    # Predict waste type
    waste_type = cnn_model.predict(img_array)
    print("Predicted Waste Type:", waste_type)

    # Load the regression model
    weight_model = LinearRegression()  # Load your trained model here
    # Assuming we have features for the new image
    features = np.random.rand(1, 10)  # Placeholder for actual features
    predicted_weight = weight_model.predict(features)
    print("Predicted Waste Weight:", predicted_weight)

if __name__ == "__main__":
    predict_waste('path_to_new_image.jpg')
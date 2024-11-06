import tensorflow as tf
from tensorflow.keras import layers, models
import os

def create_model():
    model = models.Sequential([
        layers.Conv2D(32, (3, 3), activation='relu', input_shape=(224, 224, 3)),
        layers.MaxPooling2D(pool_size=(2, 2)),
        layers.Conv2D(64, (3, 3), activation='relu'),
        layers.MaxPooling2D(pool_size=(2, 2)),
        layers.Flatten(),
        layers.Dense(64, activation='relu'),
        layers.Dense(1, activation='sigmoid')  # Assuming binary classification
    ])
    model.compile(optimizer='adam', loss='binary_crossentropy', metrics=['accuracy'])
    return model

def train_model(image_folder='processed_images', labels_file='labels.txt'):
    model = create_model()
    # Load images and labels
    # This part requires a dataset with images and corresponding labels
    # For example, using tf.keras.preprocessing.image_dataset_from_directory

    model.fit(train_data, epochs=10, validation_data=val_data)
    model.save('waste_classifier.h5')

if __name__ == "__main__":
    train_model()
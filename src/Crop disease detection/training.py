import matplotlib.pyplot as plt
import numpy as np
import PIL
import tensorflow as tf

from tensorflow import keras
from tensorflow.keras import layers
from tensorflow.keras.models import Sequential

####### This program was used for training the dataset

# setting parameters 
batch_size = 32
img_height = 256     
img_width = 256

# training dataset with labels as the directory name
train_ds = tf.keras.utils.image_dataset_from_directory(
  directory="D:/Agri-bot data/train",
  validation_split=0.2,
  subset="training",
  seed=123,
  image_size=(img_height, img_width),
  batch_size=batch_size)

# validation dataset with labels as the directory name
val_ds = tf.keras.utils.image_dataset_from_directory(
  directory="D:/Agri-bot data/test",
  validation_split=0.2,
  subset="validation",
  seed=123,
  image_size=(img_height, img_width),
  batch_size=batch_size)

# classification categories
class_names = train_ds.class_names
#Adding a Normalisation layer
normalization_layer = layers.Rescaling(1./255)
normalized_ds = train_ds.map(lambda x, y: (normalization_layer(x), y))
image_batch, labels_batch = next(iter(normalized_ds))
first_image = image_batch[0]

# Number of Classes
num_classes = len(class_names)

# Creating the neural network
# Rescale the input image --> Series of Convolutional Layers and Max Pooling layers --> Flattening layer --> Dense Layers --> Classification
model = Sequential([
  layers.Rescaling(1./255, input_shape=(img_height, img_width, 3)),
  layers.Conv2D(16, 3, padding='same', activation='relu'),
  layers.Conv2D(16, 3, padding='same', activation='relu'),
  layers.MaxPooling2D(),
  layers.Conv2D(32, 3, padding='same', activation='relu'),
  layers.Conv2D(32, 3, padding='same', activation='relu'),
  layers.MaxPooling2D(),
  layers.Conv2D(64, 3, padding='same', activation='relu'),
  layers.Conv2D(64, 3, padding='same', activation='relu'),
  layers.MaxPooling2D(),
  layers.Flatten(),
  layers.Dense(128, activation='relu'),
  layers.Dense(128, activation='relu'),
  layers.Dense(num_classes)
])

# Compiling the model
model.compile(optimizer='adam',
              loss=tf.keras.losses.SparseCategoricalCrossentropy(from_logits=True),
              metrics=['accuracy'])
epochs=4

# Training the model
history = model.fit(
  train_ds,
  validation_data=val_ds,
  epochs=epochs
)

# Printing evaluation
acc = history.history['accuracy']
val_acc = history.history['val_accuracy']

loss = history.history['loss']
val_loss = history.history['val_loss']

epochs_range = range(epochs)

# Save the trained model
model.save("model.h5")
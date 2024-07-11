

import cv2
import mediapipe as mp
import matplotlib.pyplot as plt
import numpy as np

mp_hands = mp.solutions.hands
hands = mp_hands.Hands()
mp_draw = mp.solutions.drawing_utils

thumb_positions = []

cap = cv2.VideoCapture(0)

while True:
    success, frame = cap.read()
    if not success:
        break
    frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    results = hands.process(frame_rgb)
    if results.multi_hand_landmarks:
        for hand_landmarks in results.multi_hand_landmarks:
            mp_draw.draw_landmarks(frame, hand_landmarks, mp_hands.HAND_CONNECTIONS)
            thumbtip = hand_landmarks.landmark[mp_hands.HandLandmark.THUMB_TIP]
            thumbtip_x, thumbtip_y = int(thumbtip.x * frame.shape[1]), int(thumbtip.y * frame.shape[0])
            thumb_x, thumb_y = int(thumbtip.x*frame.shape[1]), int(thumbtip.y * frame.shape[0])
            thumb_positions.append((thumb_x, thumb_y))
    cv2.imshow("Thumb Movement", frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
        cap.release()
        cv2.destroyAllWindows()

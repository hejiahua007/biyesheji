import cv2
import numpy as np
import onnxruntime as ort
import time
from collections import Counter
import os
import speaking
from clock import Alarm_add,Alarm_del
# 环境检测模型加载
env_model_path = r"/home/pi/Desktop/sleep/file/model/huangjing.onnx"
env_net = ort.InferenceSession(env_model_path)

# 手机书本模型加载
phone_book_model_path = r"/home/pi/Desktop/sleep/file/model/5.onnx"
phone_book_net = ort.InferenceSession(phone_book_model_path)

# 标签字典
env_labels = {0: 'dark', 1: 'dim', 2: 'bright'}
phone_book_labels = {0: 'book', 1: 'phone'}

# 模型参数（根据您的模型调整）
model_h = 320
model_w = 320
nl = 3
na = 3
stride = [8., 16., 32.]
anchors = [[10, 13, 16, 30, 33, 23], [30, 61, 62, 45, 59, 119], [116, 90, 156, 198, 373, 326]]
anchor_grid = np.asarray(anchors, dtype=np.float32).reshape(nl, -1, 2)

def plot_one_box(x, img, color=None, label=None, line_thickness=None):
    tl = line_thickness or round(0.002 * (img.shape[0] + img.shape[1]) / 2) + 1
    color = color or [random.randint(0, 255) for _ in range(3)]
    x = x.squeeze()
    c1, c2 = (int(x[0]), int(x[1])), (int(x[2]), int(x[3]))
    cv2.rectangle(img, c1, c2, color, thickness=tl, lineType=cv2.LINE_AA)
    if label:
        tf = max(tl - 1, 1)
        t_size = cv2.getTextSize(label, 0, fontScale=tl / 3, thickness=tf)[0]
        c_center = (c1[0] + c2[0]) // 2, (c1[1] + c2[1]) // 2
        label_pos = c_center[0] - t_size[0] // 2, c_center[1] + t_size[1] // 2
        cv2.rectangle(img, (label_pos[0], label_pos[1] - t_size[1] - 3), (label_pos[0] + t_size[0], label_pos[1]), color, -1, cv2.LINE_AA)
        cv2.putText(img, label, (label_pos[0], label_pos[1] - 2), 0, tl / 3, [225, 255, 255], thickness=tf, lineType=cv2.LINE_AA)

def infer_img(img, net, model_h, model_w, nl, na, stride, anchor_grid, thred_nms=0.4, thred_cond=0.5):
    img_resized = cv2.resize(img, (model_w, model_h), interpolation=cv2.INTER_AREA)
    img_rgb = cv2.cvtColor(img_resized, cv2.COLOR_BGR2RGB)
    img_normalized = img_rgb.astype(np.float32) / 255.0
    blob = np.expand_dims(np.transpose(img_normalized, (2, 0, 1)), axis=0)
    outs = net.run(None, {net.get_inputs()[0].name: blob})[0].squeeze(axis=0)
    outs = cal_outputs(outs, nl, na, model_w, model_h, anchor_grid, stride)
    img_h, img_w, _ = img.shape
    boxes, confs, ids = post_process_opencv(outs, model_h, model_w, img_h, img_w, thred_nms, thred_cond)
    return boxes, confs, ids
def _make_grid( nx, ny):
        xv, yv = np.meshgrid(np.arange(ny), np.arange(nx))
        return np.stack((xv, yv), 2).reshape((-1, 2)).astype(np.float32)
def cal_outputs(outs, nl, na, model_w, model_h, anchor_grid, stride):
    row_ind = 0
    grid = [np.zeros(1)] * nl
    for i in range(nl):
        h, w = int(model_w / stride[i]), int(model_h / stride[i])
        length = int(na * h * w)
        if grid[i].shape[2:4] != (h, w):
            grid[i] = _make_grid(w, h)
        outs[row_ind:row_ind + length, 0:2] = (outs[row_ind:row_ind + length, 0:2] * 2. - 0.5 + np.tile(grid[i], (na, 1))) * int(stride[i])
        outs[row_ind:row_ind + length, 2:4] = (outs[row_ind:row_ind + length, 2:4] * 2) ** 2 * np.repeat(anchor_grid[i], h * w, axis=0)
        row_ind += length
    return outs

def post_process_opencv(outputs, model_h, model_w, img_h, img_w, thred_nms, thred_cond):
    conf = outputs[:, 4].tolist()
    c_x = outputs[:, 0] / model_w * img_w
    c_y = outputs[:, 1] / model_h * img_h
    w = outputs[:, 2] / model_w * img_w
    h = outputs[:, 3] / model_h * img_h
    p_cls = outputs[:, 5:]
    if len(p_cls.shape) == 1:
        p_cls = np.expand_dims(p_cls, 1)
    cls_id = np.argmax(p_cls, axis=1)
    p_x1 = np.expand_dims(c_x - w / 2, -1)
    p_y1 = np.expand_dims(c_y - h / 2, -1)
    p_x2 = np.expand_dims(c_x + w / 2, -1)
    p_y2 = np.expand_dims(c_y + h / 2, -1)
    areas = np.concatenate((p_x1, p_y1, p_x2, p_y2), axis=-1)
    areas = areas.tolist()
    ids = cv2.dnn.NMSBoxes(areas, conf, thred_cond, thred_nms)
    if len(ids) > 0:
        return np.array(areas)[ids], np.array(conf)[ids], cls_id[ids]
    else:
        return [], [], []


def save_images(images, path):
    if not os.path.exists(path):
        os.makedirs(path)
    for i, img in enumerate(images):
        cv2.imwrite(os.path.join(path, f'image_{i}.jpg'), img)
        
# '开睡前状态检测': 
def start_sleep_status_detection():
    Alarm_add("23:50:00", 5)
    
# '关睡前状态检测': 
def stop_sleep_status_detection():
    Alarm_del("23:50:00", 5)

def detection():
    # 初始化摄像头
    cap = cv2.VideoCapture(0)

    env_results = []
    phone_book_results = []
    images = []

    for i in range(10):
        ret, img = cap.read()
        if not ret:
            print("摄像头读取失败")
            speaking.play_audio("../file/detection/zhiling/shexiangtouduqushibai.wav")
            continue
        images.append(img)
        
        # 环境检测
        _, env_scores, env_ids = infer_img(img, env_net, model_h, model_w, nl, na, stride, anchor_grid, thred_nms=0.5, thred_cond=0.5)
        
        if len(env_ids) > 0:
            env_label = env_labels[env_ids[0]]
            env_results.append(env_label)
        
        # 等待2秒
        time.sleep(2)

    cap.release()

    # 保存图片
    save_images(images, '/home/pi/Desktop/sleep/file/detection')

    # 环境检测结果多数决定
    env_result = Counter(env_results).most_common(1)[0][0]
    print(f"环境: {env_result}")
    
    # 根据环境结果进行下一步
    if env_result == 'dark':
        print("处于睡眠状态")
        speaking.play_audio("../file/detection/zhiling/mubiaorenwuchuyushuimianzhuangtai.wav")
    elif env_result == 'dim':
        print("睡眠环境识别为微亮。")
        speaking.play_audio("../file/detection/zhiling/shuimianhuanjingshibieweiweiliang.wav")
        # 这里可以添加一些建议
    elif env_result == 'bright':
        speaking.play_audio("../file/detection/zhiling/shuimianhuanjingshibieweimingliang.wav")
        print("开启第二阶段识别。")
        # 使用书本手机模型进行检测
        for img in images:
            det_boxes, scores, ids = infer_img(img, phone_book_net, model_h, model_w, nl, na, stride, anchor_grid, thred_nms=0.5, thred_cond=0.6)
            for box, score, id in zip(det_boxes, scores, ids):
                label = phone_book_labels[id.item()]
                phone_book_results.append(label)
                plot_one_box(box.astype(np.int), img, color=(255, 0, 0), label=f'{label}:{score:.2f}', line_thickness=None)
        
        # 检查是否有检测结果
        if phone_book_results:
            # 书本手机检测结果多数决定
            phone_book_result = Counter(phone_book_results).most_common(1)[0][0]
            print(f"状态: {phone_book_result}")
            if phone_book_result == 'book':
                speaking.play_audio("../file/detection/zhiling/jiancedaomubiaorenwukanshu.wav")
            if phone_book_result == 'phone':
                speaking.play_audio("../file/detection/zhiling/jiancedaomubiaorenwuwanshouji.wav")
            # 这里可以添加一些建议
        else:
            print("没有检测到任何对象")
            speaking.play_audio("../file/detection/zhiling/jiancebuchumubiaorenwuxingwei.wav")
            # 这里可以添加一些建议

# 运行检测函数
#detection()



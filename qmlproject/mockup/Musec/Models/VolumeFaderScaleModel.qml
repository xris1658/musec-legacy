import QtQml.Models 2.15

ListModel {
    property double maxDecibel: 6
    property double minDecibel: -144
    dynamicRoles: true
    function insertPoint(position: double, decibel: double) {
        if(position <= 0 || position >= 1 || decibel >= maxDecibel || decibel <= minDecibel) {
            return;
        }
        if(count == 0) {
            append({"position": position, "decibel": decibel});
            return;
        }
        let front = get(0);
        if(front.position > position && minDecibel < decibel && front.decibel > decibel) {
            insert(0, {"position": position, "decibel": decibel});
            return;
        }
        for(let i = 0; i < count - 1; ++i) {
            let former = get(i);
            let latter = get(i + 1);
            if(former.position < position && latter.position > position && former.decibel < decibel && latter.decibel > decibel) {
                insert(i + 1, {"position": position, "decibel": decibel});
                return;
            }
        }
        let back = get(count - 1);
        if(back.position < position && position < 1 && back.decibel < decibel && decibel < maxDecibel) {
            append({"position": position, "decibel": decibel});
            return;
        }
    }
    function removePoint(index: int) {
        remove(index, 1);
    }
    function decibelFromPosition(position: double) {
        if(position <= 0) {
            return minDecibel;
        }
        if(position >= 1) {
            return maxDecibel;
        }
        if(count == 0) {
            return minDecibel + (maxDecibel - minDecibel) * position;
        }
        if(position < get(0).position) {
            return minDecibel + position / (get(0).position) * (get(0).decibel - minDecibel);
        }
        if(position == get(0).position) {
            return get(0).decibel;
        }
        for(let i = 0; i < count - 1; ++i) {
            let former = get(i);
            let latter = get(i + 1);
            if(former.position < position && latter.position > position) {
                return former.decibel + (latter.decibel - former.decibel) * (position - former.position) / (latter.position - former.position);
            }
            if(position == latter.position) {
                return latter.decibel;
            }
        }
        if(position > get(count - 1).position) {
            return get(count - 1).decibel + (maxDecibel - get(count - 1).decibel) * (position - get(count - 1).position) / (1 - get(count - 1).position);
        }
        return NaN;
    }
    function positionFromDecibel(decibel: double) {
        if(decibel < minDecibel) {
            return 0.0;
        }
        if(decibel > maxDecibel) {
            return 1.0;
        }
        if(count == 0) {
            return (decibel - minDecibel) / (maxDecibel - minDecibel);
        }
        let first = get(0);
        if(decibel < first.decibel) {
            return first.position * (decibel - minDecibel) / (first.decibel - minDecibel);
        }
        if(decibel == first.decibel) {
            return (first.position);
        }
        for(let i = 0; i < count - 1; ++i) {
            let former = get(i);
            let latter = get(i + 1);
            if(former.decibel < decibel && latter.decibel > decibel) {
                return former.position + (latter.position - former.position) * (decibel - former.decibel) / (latter.decibel - former.decibel);
            }
            if(decibel == latter.decibel) {
                return latter.position;
            }
        }
        let last = get(count - 1);
        if(decibel > last.decibel) {
            return last.position + (1 - last.position) * (decibel - last.decibel) / (maxDecibel - last.decibel);
        }
        return NaN;
    }
}

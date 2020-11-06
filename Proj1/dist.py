import numpy as np
from subprocess import run as cmd


def run(size=None):
    args = ["./main"]
    if size is not None:
        args.append(str(size))
    res = cmd(args, capture_output=True, check=True, text=True)
    nums = [int(x) for x in res.stdout.splitlines()]
    return np.asarray(nums, dtype=np.uint8)


def count(nums):
    size = nums.size
    un, cnt = np.unique(nums, return_counts=True)
    max = np.max(un)

    ans = np.zeros(max+1)
    for i, c in zip(un, cnt):
        ans[i] = c / size
    return ans


if __name__ == "__main__":
    import sys

    if len(sys.argv) > 1:
        nums = run(int(sys.argv[1]))
    else:
        nums = run()

    for i, cnt in enumerate(count(nums)):
        print(f'{i:3d}:  {100*cnt:3.4f}')

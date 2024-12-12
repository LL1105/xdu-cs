import edu.princeton.cs.algs4.Picture;

public class SeamCarver implements SC {
    private Picture picture;          // 当前的图片
    private double[][] energyMatrix;  // 存储每个像素的能量矩阵

    // 构造函数，初始化图片并计算每个像素的能量
    public SeamCarver(Picture picture) {
        this.picture = new Picture(picture);  // 创建一个新的 Picture 对象
        energyMatrix = new double[width()][height()];  // 初始化能量矩阵
        computeEnergy();  // 计算每个像素的能量
    }

    // 返回当前的图片
    @Override
    public Picture picture() {
        return picture;
    }

    // 返回当前图片的宽度
    @Override
    public int width() {
        return picture.width();
    }

    // 返回当前图片的高度
    @Override
    public int height() {
        return picture.height();
    }

    // 计算每个像素的能量
    private void computeEnergy() {
        for (int x = 0; x < width(); x++) {
            for (int y = 0; y < height(); y++) {
                energyMatrix[x][y] = energy(x, y);  // 根据 (x, y) 计算能量
            }
        }
    }

    // 返回指定位置 (x, y) 像素的能量
    @Override
    public double energy(int x, int y) {
        int xl = x - 1;
        int xr = x + 1;
        int yu = y - 1;
        int yd = y + 1;
        if (x == 0) {
            xl = width() - 1;
        }
        if (x == width() - 1) {
            xr = 0;
        }
        if (y == 0) {
            yu = height() - 1;
        }
        if (y == height() - 1) {
            yd = 0;
        }

        // 计算 x 和 y 方向的梯度能量
        double dx = Math.pow(getColor(xl, y).getRed() - getColor(xr, y).getRed(), 2)
                + Math.pow(getColor(xl, y).getGreen() - getColor(xr, y).getGreen(), 2)
                + Math.pow(getColor(xl, y).getBlue() - getColor(xr, y).getBlue(), 2);

        double dy = Math.pow(getColor(x, yu).getRed() - getColor(x, yd).getRed(), 2)
                + Math.pow(getColor(x, yu).getGreen() - getColor(x, yd).getGreen(), 2)
                + Math.pow(getColor(x, yu).getBlue() - getColor(x, yd).getBlue(), 2);

        return Math.sqrt(dx + dy);  // 返回能量值（梯度和的平方根）
    }

    // 获取指定位置 (x, y) 的颜色
    private java.awt.Color getColor(int x, int y) {
        return picture.get(x, y);  // 返回颜色
    }

    // 返回水平 seam 的像素序列（通过转置图片，调用垂直 seam 方法来实现）
    @Override
    public int[] findHorizontalSeam() {
        // 通过转置图片来计算水平 seam，转置后调用 findVerticalSeam 来获取
        Picture transposed = transpose(picture);
        SeamCarver transposedCarver = new SeamCarver(transposed);
        return transposedCarver.findVerticalSeam();
    }

    // 返回垂直 seam 的像素序列
    @Override
    public int[] findVerticalSeam() {
        // 使用动态规划计算最佳垂直 seam
        double[][] distTo = new double[width()][height()];  // 距离矩阵
        int[][] edgeTo = new int[width()][height()];  // 边界矩阵

        // 初始化第一行的 distTo，值为能量矩阵的值
        for (int x = 0; x < width(); x++) {
            distTo[x][0] = energyMatrix[x][0];
        }

        // 填充 distTo 和 edgeTo 矩阵
        for (int y = 1; y < height(); y++) {
            for (int x = 0; x < width(); x++) {
                double minDist = distTo[x][y - 1];  // 默认取上方像素的最短距离
                int minEdge = x;  // 默认的边界是当前列

                // 如果左上角的像素距离较小，则更新
                if (x > 0 && distTo[x - 1][y - 1] < minDist) {
                    minDist = distTo[x - 1][y - 1];
                    minEdge = x - 1;
                }

                // 如果右上角的像素距离较小，则更新
                if (x < width() - 1 && distTo[x + 1][y - 1] < minDist) {
                    minDist = distTo[x + 1][y - 1];
                    minEdge = x + 1;
                }

                distTo[x][y] = energyMatrix[x][y] + minDist;  // 当前像素的距离值
                edgeTo[x][y] = minEdge;  // 记录最短路径的边界
            }
        }

        // 反向重构 seamt
        int[] seam = new int[height()];
        int minX = 0;
        // 找到最后一行中最小距离的位置
        for (int x = 1; x < width(); x++) {
            if (distTo[x][height() - 1] < distTo[minX][height() - 1]) {
                minX = x;
            }
        }

        // 从最后一行反向追踪 seam
        for (int y = height() - 1; y >= 0; y--) {
            seam[y] = minX;
            minX = edgeTo[minX][y];  // 更新 minX，沿着路径回溯
        }

        return seam;  // 返回垂直 seam
    }

    // 移除水平 seam
    @Override
    public void removeHorizontalSeam(int[] seam) {
        if (seam == null || seam.length != width()) {
            throw new IllegalArgumentException("Invalid seam array.");  // 如果 seam 无效，抛出异常
        }

        // 创建一个新的图片，宽度保持不变，高度减少 1
        Picture newPicture = new Picture(width(), height() - 1);

        // 遍历每一列，移除水平 seam
        for (int x = 0; x < width(); x++) {
            int seamY = seam[x];  // 获取当前列对应的 seam 行索引

            // 确保 seamY 索引在有效范围内
            if (seamY < 0 || seamY >= height()) {
                throw new IllegalArgumentException("Invalid seam index at column " + x);
            }

            // 将 seam 行以上的像素复制到新图片
            for (int y = 0; y < seamY; y++) {
                newPicture.set(x, y, picture.get(x, y));
            }

            // 将 seam 行以下的像素复制到新图片
            for (int y = seamY + 1; y < height(); y++) {
                newPicture.set(x, y - 1, picture.get(x, y));
            }
        }

        picture = newPicture;  // 更新图片为新图片
        computeEnergy();  // 重新计算新图片的能量
    }

    // 移除垂直 seam
    @Override
    public void removeVerticalSeam(int[] seam) {
        Picture newPicture = new Picture(width() - 1, height());
        // 遍历每一行，移除垂直 seam
        for (int y = 0; y < height(); y++) {
            int seamX = seam[y];  // 获取当前行对应的 seam 列索引
            // 将 seam 列之前的像素复制到新图片
            for (int x = 0; x < seamX; x++) {
                newPicture.set(x, y, picture.get(x, y));
            }
            // 将 seam 列之后的像素复制到新图片
            for (int x = seamX + 1; x < width(); x++) {
                newPicture.set(x - 1, y, picture.get(x, y));
            }
        }
        picture = newPicture;  // 更新图片为新图片
        computeEnergy();  // 重新计算新图片的能量
    }

    // 转置图片的方法
    private Picture transpose(Picture pic) {
        Picture transposed = new Picture(pic.height(), pic.width());
        // 将图片的每个像素转置
        for (int x = 0; x < pic.width(); x++) {
            for (int y = 0; y < pic.height(); y++) {
                transposed.set(y, x, pic.get(x, y));
            }
        }
        return transposed;  // 返回转置后的图片
    }
}

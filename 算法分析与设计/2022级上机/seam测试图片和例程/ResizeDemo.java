/* *****************************************************************************
 *  Compilation:  javac-algs4 ResizeDemo.java
 *  Execution:    java-algs4 ResizeDemo input.png columnsToRemove rowsToRemove
 *  Dependencies: SeamCarver.java
 *
 *
 *  Read image from file specified as command line argument. Use SeamCarver
 *  to remove number of rows and columns specified as command line arguments.
 *  Display the image and print time elapsed.
 *
 *  % java-algs4 ResizeDemo HJoceanSmall.png 150 0
 *
 **************************************************************************** */

import edu.princeton.cs.algs4.Picture;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.Stopwatch;

public class ResizeDemo {
    public static void main(String[] args) {
        if (args.length != 3) {
            StdOut.println(
                    "Usage:\njava ResizeDemo [image filename] [num columns to remove] [num rows to remove]");
            return;
        }

        Picture picture = new Picture(args[0]);
        int removeColumns = Integer.parseInt(args[1]);
        int removeRows = Integer.parseInt(args[2]);

        StdOut.printf("%d-by-%d image\n", picture.width(), picture.height());
        SeamCarver sc = new SeamCarver(picture);

        Stopwatch sw = new Stopwatch();

        for (int i = 0; i < removeRows; i++) {
            int[] horizontalSeam = sc.findHorizontalSeam();
            sc.removeHorizontalSeam(horizontalSeam);
        }
        System.out.println("height:" + sc.height());
        for (int i = 0; i < removeColumns; i++) {
            int[] verticalSeam = sc.findVerticalSeam();
            sc.removeVerticalSeam(verticalSeam);
        }

        StdOut.printf("new image size is %d columns by %d rows\n", sc.width(), sc.height());

        StdOut.println("Resizing time: " + sw.elapsedTime() + " seconds.");
        picture.show();
        sc.picture().show();
    }

}

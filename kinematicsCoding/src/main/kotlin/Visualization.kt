import java.awt.*
import java.awt.event.MouseEvent
import java.awt.event.MouseMotionListener
import javax.swing.JComponent
import javax.swing.JFrame
import kotlin.math.pow


fun main() {
    val armVisualization = ArmVisualization()
    armVisualization.preferredSize = Dimension(1000, 1000)

    val frame = JFrame()
    frame.defaultCloseOperation = JFrame.DISPOSE_ON_CLOSE
    frame.contentPane.add(armVisualization, BorderLayout.CENTER)
    frame.pack()
    frame.isVisible = true

}

class ArmVisualization: JComponent(), MouseMotionListener {
    init {
        addMouseMotionListener(this)
    }

    override fun paintComponent(g: Graphics) {
        super.paintComponent(g)
        g as Graphics2D
        fun drawLine(a: Point2D, b: Point2D) {
            g.drawLine( a.x.toInt(), a.y.toInt(),
                        b.x.toInt(), b.y.toInt())
        }

        
        val offset = Point2D(size.getWidth()/2,size.getHeight()/2)

        val threeSegmentArmKinematics = ThreeSegmentArmKinematics(180.0, 150.0, 30.0)
        val rootPoint = Point2D(0.0, 0.0)
        val endpointNotFlipped = mousePoint - offset
        val endpoint = Point2D(endpointNotFlipped.x, -endpointNotFlipped.y)
        val endAngleRad = Math.toRadians(90.0)//rootPoint.angleToPointRad(endpoint)
        println("endpoint: $endpoint")
        val armState = threeSegmentArmKinematics.calculateArmState(endpoint, endAngleRad)

        val pointsToDraw = listOf(rootPoint) + armState.asList()
        pointsToDraw.map{Point2D(it.x, -it.y)}.map{ it + offset }.reversed().reduceIndexed { i, acc, it ->
            g.stroke = BasicStroke(4f * i)
            g.color = Color((i*100).toDouble().pow(2.0).toInt())
            drawLine(acc, it)
            it
        }
    }

    var mousePoint: Point2D = Point2D(1.0, 1.0)

    override fun mouseDragged(e: MouseEvent?) {
        mousePoint = Point2D(e?.x?.toDouble() ?: 0.0, e?.y?.toDouble() ?: 0.0)
        repaint()
    }
    override fun mouseMoved(e: MouseEvent?) {}
}
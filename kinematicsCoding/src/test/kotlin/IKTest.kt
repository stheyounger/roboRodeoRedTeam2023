import v1.Point2D
import v1.ThreeSegmentArmKinematics

fun main() {
    val threeSegmentArmKinematics = ThreeSegmentArmKinematics(10.0, 10.0, 10.0)

    val endpoint = Point2D(20.0, 10.0)
    val endAngleRad = Math.toRadians(90.0)
    println("endpoint: $endpoint")
    val armState = threeSegmentArmKinematics.calculateArmState(endpoint, endAngleRad)
    println("armState: $armState")
}
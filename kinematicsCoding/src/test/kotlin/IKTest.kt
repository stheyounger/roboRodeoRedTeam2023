fun main() {
    val threeSegmentArmKinematics = ThreeSegmentArmKinematics(180.0, 150.0, 30.0)

    val endpoint = Point2D(-24.0, 35.5)
    val endAngleRad = Math.toRadians(90.0)
    println("endpoint: $endpoint")
    val armState = threeSegmentArmKinematics.calculateArmState(endpoint, endAngleRad)
    println("armState: $armState")
}
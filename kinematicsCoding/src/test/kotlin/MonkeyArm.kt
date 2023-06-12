import kotlin.math.PI

class MonkeyArm: ThreeSegmentArm {
    override val segmentALength: Double = 1.0
    override val segmentBLength: Double = 1.0
    override val segmentCLength: Double = 1.0

    override val aAngleLimitsRadians: ClosedFloatingPointRange<Double> = 0.0..PI
    override val bAngleLimitsRadians: ClosedFloatingPointRange<Double> = 0.0..PI
    override val cAngleLimitsRadians: ClosedFloatingPointRange<Double> = 0.0..PI

    val kinematics = ThreeSegmentArmKinematics(this)
}
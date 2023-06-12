//Arm interface deals in relative angles
interface ThreeSegmentArm {
    val segmentALength: Double
    val segmentBLength: Double
    val segmentCLength: Double

    val aAngleLimitsRadians: ClosedFloatingPointRange<Double>
    val bAngleLimitsRadians: ClosedFloatingPointRange<Double>
    val cAngleLimitsRadians: ClosedFloatingPointRange<Double>

    val maxReach: Double
        get() = segmentALength + segmentBLength + segmentCLength
}

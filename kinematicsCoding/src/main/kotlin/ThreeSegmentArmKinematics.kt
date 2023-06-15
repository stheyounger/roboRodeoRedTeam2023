import kotlin.math.PI
import kotlin.math.acos
import kotlin.math.pow

//inputs:
//final position
//final angle
//limits

//Unknowns:
//segmentPoints

//deals in points
class ThreeSegmentArmKinematics(
    private val threeSegmentArm: ThreeSegmentArm
) {

    val root = Point2D(0.0, 0.0)

    data class Positions(val a: Point2D, val b: Point2D, val c: Point2D){
        fun asList(): List<Point2D> = listOf(a, b, c)
        override fun toString(): String = "Positions(a: $a, b: $b, c: $c)"
    }

    fun solvePositions(target: Point2D, targetAngle: Double): Positions {
        val distanceToTarget = root.distanceTo(target)
        if (isJointCOutOfBounds(distanceToC= distanceToTarget)) {
            println("C is out of bounds!")
            return Positions(c= Point2D(Double.POSITIVE_INFINITY, Double.POSITIVE_INFINITY), b= Point2D(Double.POSITIVE_INFINITY, Double.POSITIVE_INFINITY), a= Point2D(Double.POSITIVE_INFINITY, Double.POSITIVE_INFINITY))
        }

        val b = target + Point2D.pointFromVector(   magnitude= threeSegmentArm.segmentCLength,
                                                    directionRadians= (targetAngle + PI))
        val maxReachOfB = threeSegmentArm.segmentALength + threeSegmentArm.segmentBLength
        val distanceToB = root.distanceTo(b)
        val minDistanceToB = threeSegmentArm.segmentALength - threeSegmentArm.segmentBLength
        if (isJointBOutOfBounds(distanceToB= distanceToB,
                                minDistanceToB= minDistanceToB,
                                maxReachOfB= maxReachOfB)) {
            println("B is out of bounds!")
            return Positions(c= target, b= Point2D(Double.POSITIVE_INFINITY, Double.POSITIVE_INFINITY), a= Point2D(Double.POSITIVE_INFINITY, Double.POSITIVE_INFINITY))
        }

        val directionToB = root.angleToRadians(b)
        val angleOppositeSegmentBInTriangleRadians = angleRadiansFromSideLengths(   oppositeSide= threeSegmentArm.segmentBLength,
                                                                                    otherSideA= distanceToB,
                                                                                    otherSideB= threeSegmentArm.segmentALength)
        val directionToA = directionToB + angleOppositeSegmentBInTriangleRadians
        val a = root + Point2D.pointFromVector( magnitude= threeSegmentArm.segmentALength,
                                                directionRadians = directionToA)
        val distanceToA = root.distanceTo(a)
        if (isJointAOutOfBounds(distanceToA= distanceToA))  {
            println("A is out of reach!\ndistanceToA: $distanceToA\nsegmentALength: ${threeSegmentArm.segmentALength}")
            return Positions(c= target, b= b, a= Point2D(Double.POSITIVE_INFINITY, Double.POSITIVE_INFINITY))
        }

        return Positions(c= target, b= b, a= a)
    }

    private fun isJointCOutOfBounds(distanceToC: Double): Boolean {
        return distanceToC > threeSegmentArm.maxReach
    }

    private fun isJointBOutOfBounds(distanceToB: Double, minDistanceToB: Double, maxReachOfB: Double): Boolean {
        return when {
            distanceToB > maxReachOfB -> true
            distanceToB < minDistanceToB -> true
            else -> false
        }
    }

    private fun isJointAOutOfBounds(distanceToA: Double): Boolean {
        val error = 0.0001
        return distanceToA !in threeSegmentArm.segmentALength-error..threeSegmentArm.segmentALength+error
    }

    private fun angleRadiansFromSideLengths(oppositeSide: Double, otherSideA: Double, otherSideB: Double): Double {
        return acos((otherSideA.pow(2) + otherSideB.pow(2) - oppositeSide.pow(2)) / (2 * otherSideA * otherSideB))
    }
}
